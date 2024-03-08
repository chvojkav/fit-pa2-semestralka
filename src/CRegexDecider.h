//------------------------------------------------------------------------------
// File: CRegexDecider.h
// Author: Chvojka Vojtech, CTU Prague
// Enables only urls that match regular expression.
//------------------------------------------------------------------------------
#pragma once
#include <regex>
#include <string>

#include "IUrlDecider.h"

/// @brief Enables only urls that match regular expression.
class CRegexDecider : public IUrlDecider
{
public:
    /// @param[in] regex URL regular expression to be used.
    CRegexDecider(const std::string& regex);

    /// @see IUrlDecider.
    virtual bool DecideUrl(const char* pUrl) override;
private:
    /// @brief URL regular expression.
    std::regex m_regex;
};
