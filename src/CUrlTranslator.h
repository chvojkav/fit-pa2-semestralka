//------------------------------------------------------------------------------
// File: CUrlTranslator.h
// Author: Chvojka Vojtech, CTU Prague
// Utility for
//     - converting relative URL to absolute URL.
//     - converting absolute URL to absolute file path.
//     - converting absolute URL to relative URL.
//------------------------------------------------------------------------------
#pragma once
#include <string>
#include <filesystem>


/// @brief Utility for various conversions of URL.
class CUrlTranslator
{
public:
    /// @param[in] outDir Output directory to be used to get absolute file path.
    CUrlTranslator(const std::string& outDir);

    /// @brief Creates absolute URL from current 'root' URL and relative URL.
    ///
    /// @warning Does not parse query or fragment part of relative URL. Those
    /// are kept from root URL.
    ///
    /// @param[in] rootUrl Current URL.
    /// @param[in] relativeUrl URL relative to 'root' URL.
    /// @return Absolute URL of relative URL.
    std::string GetAbsoluteUrl(const std::string& rootUrl,
                               const std::string& relativeUrl) const;

    /// @brief Gets absolute file path from URL.
    ///
    /// @param[in] url URL.
    /// @return Absolute file path.
    std::filesystem::path GetAbsouluteFilePath(const std::string& url) const;

    /// @brief Gets relative file path from current 'root' URL to absolute URL.
    ///
    /// @param[in] rootUrl Current URL.
    /// @param[in] absoluteUrl Absolute URL.
    /// @return Relative path.
    std::filesystem::path GetRelativeFilePath(const std::string& rootUrl,
                                              const std::string& absoluteUrl) const;

private:
    /// @brief Output directory to be used to get absolute file path.
    std::string m_outDir;
};
