//------------------------------------------------------------------------------
// File: CNegationDecider.h
// Author: Chvojka Vojtech, CTU Prague
// Represents negation of another URL decider.
//------------------------------------------------------------------------------
#pragma once
#include <memory>

#include "IUrlDecider.h"

/// @brief Decider that returns negation of nested-decider's decision.
class CNegationDecider : public IUrlDecider
{
private:
    /// @brief Pointer to URL decider interface type.
    typedef std::shared_ptr<IUrlDecider> SIUrlDecider_t;
public:
    /// @param[in] pUrlDecider Nested URL decider.
    CNegationDecider(SIUrlDecider_t pUrlDecider);

    /// @see IUrlDecider.
    virtual bool DecideUrl(const char* pUrl) override;
private:
    /// @brief Pointer to nested URL decider.
    SIUrlDecider_t m_pUrlDecider;
};
