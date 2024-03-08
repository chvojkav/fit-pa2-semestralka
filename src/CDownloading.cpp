//------------------------------------------------------------------------------
// File: CDownloading.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include <cstring>
#include <netdb.h>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "CLog.h"
#include "CUrl.h"


#include "CDownloading.h"

using namespace std;

/// @brief Builds HTTP GET request from url.
///
/// @param[in] url Target URL.
/// @return Built GET request.
static string _BuildHttpGetFromUrl(const CUrl& url);

void CDownloading::Run()
{
    CUrl url(m_metadata->m_url);
    string httpGet = _BuildHttpGetFromUrl(url);

    struct hostent * host = gethostbyname(url.authority.host.c_str());

    if (host == nullptr)
    {
        THROW(CActionError, "Failed to resolve hostname: \"" << url.authority.host << "\".");
    }

    sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    memcpy(&addr.sin_addr, host->h_addr_list[0], host->h_length);

    uint16_t portNativeByteorder = 80;

    if (url.authority.hasPort())
    {
        portNativeByteorder = stoul(url.authority.port);
    }
    addr.sin_port = htons(portNativeByteorder);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock != -1
        && connect(sock, (sockaddr*)&addr, sizeof(addr)) == 0)
    {
        int res = send(sock, httpGet.c_str(), httpGet.size(), 0);
        if (-1 != res && static_cast<size_t>(res) == httpGet.size())
        {
            char buff[512];
            ssize_t bytesCnt;
            fd_set fdSet;
            FD_ZERO(&fdSet);
            FD_SET(sock, &fdSet);
            timeval tv;
            do
            {
                tv.tv_sec = 2;
                tv.tv_usec = 0;
                if (select(sock + 1, &fdSet, nullptr, nullptr, &tv) == -1)
                {
                    THROW(CActionError, "Select failure.");
                }
                if (!FD_ISSET(sock, &fdSet))
                {
                    // Timeout.
                    break;
                }

                bytesCnt = read(sock, buff, 512);
                for (ssize_t i = 0; i < bytesCnt; i++)
                {
                    m_data.push_back(buff[i]);
                }
            } while (bytesCnt > 0);
        }
    }
    close(sock);
}

void CDownloading::OnActionFinish()
{
    m_mediator->OnDownloadFinished(this);
}

void CDownloading::PrintSelf(ostream& os) const
{
    os << "Download of " << m_metadata->m_url;
}

static string _BuildHttpGetFromUrl(const CUrl& url)
{
    stringstream getStream;

    getStream << "GET " << url.path << " HTTP/1.1\r\nHost: " << url.authority.host
        << "\r\nAccept: text/html\r\n\r\n";

    return getStream.str();
}
