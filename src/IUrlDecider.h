//------------------------------------------------------------------------------
// File: IUrlDecider.h
// Author: Chvojka Vojtech, CTU Prague
// Provides interface to URL decider - accepts/declines further processing of 
// data on URL.
//------------------------------------------------------------------------------
#pragma once

/// @brief Interface for accepting/declining of URLs.
class IUrlDecider
{
public:
    virtual ~IUrlDecider() = default;

    /// @brief Decides whether given URL shoudl be accepted or declined.
    ///
    /// @param[in] pUrl Url to be decided.
    ///
    /// @retval true URL was accepted.
    /// @retval false URL was declinde.
    virtual bool DecideUrl(const char* pUrl) = 0;
};
