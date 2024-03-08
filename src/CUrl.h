//------------------------------------------------------------------------------
// File: CUrl.h
// Author: Chvojka Vojtech, CTU Prague
// Utility for validation, parsing, merging and composition of urls.
//------------------------------------------------------------------------------
#pragma once
#include <string>

#include "CUrlAuthority.h"

/// @brief Utility class for validating, parsing, meging and composing URLs.
struct CUrl
{
    /// @brief Validates that uriUrl matches URL regular expression and then
    /// parses it
    ///
    /// @param[in] uriUrl URL in URI format.
    explicit CUrl(const std::string& uriUrl);

    /// @brief Scheme part of URL. f.e. http, https, file, mailto
    std::string scheme;

    /// @brief Authority part of URL.
    CUrlAuthority authority;

    /// @brief Path part of URL.
    std::string path;

    /// @brief Query part of URL.
    std::string query;

    /// @brief Fragment part of URL.
    std::string fragment;

    /// @brief Composes URL from its members.
    ///
    /// @return Composed URL.
    explicit operator std::string() const;

    /// @brief Creates absolute URL from current 'root' URL and relative URL.
    ///
    /// @warning Does not parse query or fragment part of relative URL. Those
    /// are kept from root URL.
    ///
    /// @param[in] root Current URL.
    /// @param[in] relativeUrl URL relative to 'root' URL.
    /// @return Absolute URL of relative URL.
    static CUrl FromRelativeUrl(const CUrl& root, const std::string& relativeUrl);
};
