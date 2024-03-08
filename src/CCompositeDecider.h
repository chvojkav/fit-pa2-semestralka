//------------------------------------------------------------------------------
// File: CCompositeDecider.h
// Author: Chvojka Vojtech, CTU Prague
// Composite of IUrlDecider. See 'composite' design pattern.
//------------------------------------------------------------------------------
#pragma once
#include <memory>
#include <vector>

#include "IUrlDecider.h"

/// @brief Composite of IUrlDecider. Deciders are connected with logical AND.
class CCompositeDecider : public IUrlDecider
{
private:
    /// @brief Pointer to UrlDecider interface type.
    typedef std::shared_ptr<IUrlDecider> SIUrlDecider_t;
public:
    /// @see IUrlDecider.
    virtual bool DecideUrl(const char* pUrl) override;

    /// @brief Adds decider to composite.
    ///
    /// @param[in] pDecider Pointer to decider to be added.
    void AddDecider(SIUrlDecider_t pDecider);
private:
    /// @brief Composed deciders.
    std::vector<SIUrlDecider_t> m_deciders;
};
