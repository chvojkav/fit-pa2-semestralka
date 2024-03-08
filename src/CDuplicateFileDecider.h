//------------------------------------------------------------------------------
// File: CDuplicateFileDecider.h
// Author: Chvojka Vojtech, CTU Prague
// Enables only urls that would not overwrite existing file.
//------------------------------------------------------------------------------
#pragma once
#include <string>

#include "CUrlTranslator.h"
#include "IUrlDecider.h"

/// @brief Decider that enables only urls that would not overwrite existing
/// file.
class CDuplicateFileDecider : public IUrlDecider
{
public:
    /// @param[in] outDir Directory of output.
    CDuplicateFileDecider(const std::string& outDir);

    /// @brief see IUrlDecider.
    virtual bool DecideUrl(const char* pUrl) override;
private:
    /// @brief Translator to be used for translating URL to file path.
    CUrlTranslator m_urlTranslator;
};