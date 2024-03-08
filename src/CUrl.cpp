//------------------------------------------------------------------------------
// File: CUrl.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include <filesystem>
#include <regex>

#include "CLog.h"


#include "CUrl.h"

using namespace std;
namespace fs = std::filesystem;

/// @brief Conveniencu function for getting just schema and authority of URL.
///
/// @param[in] url URL.
/// @return Schema and authority of URL. (f.e. https://sourceforge.net)
static string _GetSchemaAndAuthority(const CUrl& url);

CUrl::CUrl(const string& uriUrl)
{
    regex urlRegex("^https?:" // mandatory scheme
                        "(\\/\\/" // begin of authority part
                            "((www.)?[-a-zA-Z0-9.]+)" // hostname
                            "(:[0-9]+)?" // port
                        ")" // end of authority part
                        "(\\/[^#?]*)?" // path part
                        "(\\?[^#]*)?" // query part
                        "(#.*)?" // fragment part
                        "$");
    if (!regex_match(uriUrl, urlRegex))
    {
        THROW(invalid_argument ,"URL does not match regex: " << uriUrl);
    }

    size_t schemeEnd = uriUrl.find(':', 0);
    size_t authorityEnd = uriUrl.find('/', schemeEnd + 3);
    if (authorityEnd == uriUrl.npos)
    {
        authorityEnd = uriUrl.size();
    }

    size_t queryBegin = uriUrl.find('?', authorityEnd);
    size_t fragmentBegin = uriUrl.find('#', authorityEnd);

    scheme = string(uriUrl, 0, schemeEnd);
    authority = CUrlAuthority(string(uriUrl, schemeEnd + 1,
                                        authorityEnd - (schemeEnd + 1)));
    
    if (fragmentBegin != uriUrl.npos)
    {
        fragment = string(uriUrl, fragmentBegin + 1,
                                uriUrl.size() - (fragmentBegin + 1));
    }
    else
    {
        fragmentBegin = uriUrl.size();
    }

    if (queryBegin != uriUrl.npos)
    {
        query = string(uriUrl, queryBegin + 1,
                            fragmentBegin - (queryBegin + 1));
    }
    else
    {
        queryBegin = fragmentBegin;
    }

    path = string(uriUrl, authorityEnd, queryBegin - authorityEnd);
}

CUrl::operator string() const
{
    return _GetSchemaAndAuthority(*this) + path
        + (query.empty() ? "" : "?") + query
        + (fragment.empty() ? "" : "#") + fragment;
}

CUrl CUrl::FromRelativeUrl(const CUrl& root, const string& relativeUrl)
{
    string tmp;
    if (relativeUrl[0] == '/')
    {
        tmp = _GetSchemaAndAuthority(root) + relativeUrl;
    }
    else
    {
        // Using std::filesystem::path to resolve all "./"s and "../"s
        fs::path path(root.path);
        if (!path.filename().string().empty())
        {
            // Remove filename.
            path = path.parent_path();
        }
        path /= relativeUrl;
        tmp = _GetSchemaAndAuthority(root) + path.lexically_normal().string();
    }

    return CUrl(tmp);
}

static string _GetSchemaAndAuthority(const CUrl& url)
{
    return url.scheme + ':' + (string)url.authority;
}
