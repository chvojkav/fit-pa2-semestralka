//------------------------------------------------------------------------------
// File: CUrlTranslator.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include <algorithm>

#include "CUrl.h"


#include "CUrlTranslator.h"

using namespace std;
using namespace std::filesystem;

/// @brief Utility function for getting common prefix of two strings.
///
/// @param[in] a First string.
/// @param[in] b Second string.
/// @return Length of common prefix.
static size_t _GetCommonPrefixLen(const string& a, const string& b);

CUrlTranslator::CUrlTranslator(const string& outDir)
    : m_outDir(outDir)
{}

string CUrlTranslator::GetAbsoluteUrl(const string& rootUrl,
                                      const string& relativeUrl) const
{
    if (relativeUrl.rfind("http", 0) != string::npos
        || relativeUrl.rfind("www", 0) != string::npos)
    {
        return relativeUrl;
    }

    CUrl absUrl = CUrl::FromRelativeUrl(CUrl(rootUrl), relativeUrl);
    return (string)absUrl;
}

path CUrlTranslator::GetAbsouluteFilePath(const string& absoluteUrl) const
{
    return absolute(path(m_outDir) / GetRelativeFilePath("", absoluteUrl));
}

path CUrlTranslator::GetRelativeFilePath(const string& rootUrl,
                                         const string& absoluteUrl) const
{
    size_t prefixLen = _GetCommonPrefixLen(rootUrl, absoluteUrl);
    string ret = string(absoluteUrl.begin() + prefixLen, absoluteUrl.end());
    if (ret.rfind("http", 0) != string::npos)
    {
        string tmp(ret.begin() + ret.find("//") + 2, ret.end());
        ret = move(tmp);
    }

    if (ret.rfind("www.", 0) != string::npos)
    {
        string tmp(ret.begin() + 4, ret.end());
        ret = move(tmp);
    }

    if (*ret.crbegin() == '/')
    {
        ret += "sortofindex.html";
    }

    return ret;
}

static size_t _GetCommonPrefixLen(const string& a, const string& b)
{
    size_t i = 0;
    size_t minLen = min(a.size(), b.size());
    for (; i < minLen; i++)
    {
        if (a[i] != b[i])
        {
            break;
        }
    }

    return i;
}
