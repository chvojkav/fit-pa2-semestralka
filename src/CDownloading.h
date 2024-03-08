//------------------------------------------------------------------------------
// File: Downloading.h
// Author: Chvojka Vojtech, CTU Prague
// Provides implementation of download action. Downloads into RAM.
//------------------------------------------------------------------------------
#pragma once
#include <vector>

#include "CAction.h"

/// @brief Action of downloading HTTP document.
class CDownloading : public CAction
{
public:
    /// @brief Downloads HTTP document on m_metadata->m_url.
    /// @see IAction.
    virtual void Run() override;

    /// @see IAction.
    virtual void OnActionFinish() override;

    /// @see IAction.
    virtual void PrintSelf(std::ostream& os) const override;

    /// @brief Downloaded document will be stored here.
    std::vector<uint8_t> m_data;
};
