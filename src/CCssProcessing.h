//------------------------------------------------------------------------------
// File: CCssProcessing.h
// Author: Chvojka Vojtech, CTU Prague
// Action that processes Css data.
//------------------------------------------------------------------------------
#pragma once
#include <string>
#include <vector>

#include "CProcessing.h"
#include "IUrlDecider.h"

/// @brief Unused.
class CCssProcessing : public CProcessing
{
public:
    CCssProcessing(const std::string& html,
                   const std::shared_ptr<IUrlDecider>& decider);

    virtual ~CCssProcessing() = default;

    virtual void Process() override;

    virtual void OnActionFinish() override;

    virtual void PrintSelf(std::ostream& os) const override;
private:
    std::string m_html;
    std::vector<std::string> m_foundLinks;
    std::shared_ptr<IUrlDecider> m_decider;
};
