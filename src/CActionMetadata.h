//------------------------------------------------------------------------------
// File: CActionMetadata.h
// Author: Chvojka Vojtech, CTU Prague
// Assembles metadata (f.e. from where was this downloaded, where it will be
// stored)
//------------------------------------------------------------------------------
#pragma once
#include <string>

/// @brief Metadata of document.
class CActionMetadata
{
public:
    /// @param[in] recursionDepth Recursion depth of this document in HTML
    /// downloading graph.
    /// @param[in] url URL from which was the document downloaded.
    CActionMetadata(size_t recursionDepth, std::string url)
        : m_recursionDepth(recursionDepth), m_url(url)
    {}

    /// @brief Recursion depth of this document in HTML downloading graph.
    size_t m_recursionDepth;

    /// @brief Name of file where will be the document stored.
    std::string m_filename;

    /// @brief URL from which was the document downloaded.
    std::string m_url;
};
