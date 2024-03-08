//------------------------------------------------------------------------------
// File: CUrlAuthority.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include "CUrlAuthority.h"

using namespace std;

CUrlAuthority::CUrlAuthority(const string& auth)
{
    size_t userinfoEnd = auth.find('@', 2);
    if (userinfoEnd != auth.npos)
    {
        userinfo = string(auth, 2, userinfoEnd - 2);
    }
    else
    {
        userinfoEnd = 1;
    }

    size_t portBegin = auth.find(':', userinfoEnd);
    if (portBegin != auth.npos)
    {
        port = string(auth, portBegin + 1, auth.size() - portBegin - 1);
    }
    else
    {
        portBegin = auth.size();
    }

    host = string(auth, userinfoEnd + 1, portBegin - (userinfoEnd + 1));
}

CUrlAuthority::operator string() const
{
    string ret("//");
    if (this->hasUserInfo())
    {
        ret += userinfo + '@';
    }

    ret += host;

    if (this->hasPort())
    {
        ret += ':' + port;
    }

    return ret;
}

bool CUrlAuthority::hasUserInfo() const
{
    return !userinfo.empty();
}

bool CUrlAuthority::hasPort() const
{
    return !port.empty();
}
