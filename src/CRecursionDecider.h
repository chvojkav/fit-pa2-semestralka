//------------------------------------------------------------------------------
// File: CRecursionDecider.h
// Author: Chvojka Vojtech, CTU Prague
// Enables only urls that do not excess allowed recursion depth.
//------------------------------------------------------------------------------
#pragma once
#include <cstddef>

#include "IUrlDecider.h"

/// @brief Enables only urls that do not excess allowed recursion depth.
class CRecursionDecider : public IUrlDecider
{
public:
    /// @param[in] recursionDepth Current recursion depth.
    /// @param[in] allowedDepth Maximal recursion depth.
    CRecursionDecider(size_t recursionDepth, size_t allowedDepth);

    /// @see IUrlDecider.
    virtual bool DecideUrl(const char* pUnused) override;
private:
    /// @brief Current recursion depth.
    size_t m_recursionDepth;

    /// @brief Maximal recursion depth.
    size_t m_allowedDepth;
};
