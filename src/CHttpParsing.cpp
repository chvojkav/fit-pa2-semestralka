//------------------------------------------------------------------------------
// File: CHttpParsing.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include "CLog.h"


#include "CHttpParsing.h"

using namespace std;

CHttpParsing::CHttpParsing(const string& http)
    : m_data(http)
{}

void CHttpParsing::Run()
{
    size_t httpHeaderEnd = m_data.find("\r\n\r\n", 0);
    if (httpHeaderEnd == m_data.npos)
    {
        THROW(CActionError, "Missing http header end.");
    }

    string header(m_data, 0, httpHeaderEnd + 4);

    {
        string tmp(m_data, httpHeaderEnd + 4,
                        m_data.size() - (httpHeaderEnd + 4));
        swap(m_data, tmp);
    }

    const char* stsHeader = "HTTP/1.1 ";
    const char* contentHeader = "Content-Type: ";
    const size_t contentHeaderLen = sizeof("Content-Type: ") - 1;

    size_t cursor = 0;
    while (cursor < header.size())
    {
        size_t pos = header.find("\r\n", cursor);
        if (pos == header.npos)
        {
            pos = header.size();
        }

        string line(header, cursor, pos - cursor);
        if (line.rfind(stsHeader, 0) != line.npos)
        {
            if (line != "HTTP/1.1 200 OK")
            {
                THROW(CActionError, "HTTP status not OK: \"" << line << "\".");
            }
        }
        else if (line.rfind(contentHeader, 0) != line.npos)
        {
            m_contentType = string(line, contentHeaderLen,
                                        line.size() - contentHeaderLen);
        }

        cursor = pos + 2;
    }

    if (m_contentType.size() == 0)
    {
        THROW(CActionError, "Missing HTTP header: '" << contentHeader << "'.");
    }
}

void CHttpParsing::OnActionFinish()
{
    m_mediator->OnHttpParsingFinished(this);
}

void CHttpParsing::PrintSelf(ostream& os) const
{
    os << "HTTP-parsing of " << m_metadata->m_url;
}
