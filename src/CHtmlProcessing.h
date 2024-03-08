//------------------------------------------------------------------------------
// File: CHtmlProcessing.h
// Author: Chvojka Vojtech, CTU Prague
// Implements action that processes HTML data.
//------------------------------------------------------------------------------
#pragma once
#include <memory>
#include <string>
#include <vector>

#include "CProcessing.h"
#include "IUrlDecider.h"

/// @brief Action that replaces http links in HTML tree and saves them.
class CHtmlProcessing : public CProcessing
{
public:
    /// @param[in] html HTML documetn.
    /// @param[in] pDecider Decider to be used to decide URLs.
    /// @param[in] leafUrlsToLocalFile Whether to link leaf urls to not-existing
    /// file.
    /// @param[in] downloadImages Whether to downloadImages.
    CHtmlProcessing(const std::string& html,
                    const std::shared_ptr<IUrlDecider>& pDecider,
                    bool leafUrlsToLocalFile,
                    bool downloadImages);

    /// @see CProcessing.
    virtual void Process() override;

    /// @see IAction.
    virtual void OnActionFinish() override;

    /// @see IAction.
    virtual void PrintSelf(std::ostream& os) const override;

    /// @brief After HTML is processed, found link will be here.
    std::vector<std::string> m_foundLinks;
private:
    /// @brief Pointer to URL decider.
    std::shared_ptr<IUrlDecider> m_pDecider;

    /// @brief Whether to link leaf urls to not-existing file.
    bool m_leafUrlsToLocalFile;

    /// @brief Whether to downloadImages.
    bool m_downloadImages;
};
