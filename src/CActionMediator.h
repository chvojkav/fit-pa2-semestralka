//------------------------------------------------------------------------------
// File: CActionMediator.h
// Author: Chvojka Vojtech, CTU Prague
// Provides 'OnFinished' callback for each type of action. Defines how actions
// trigger each other. Also provides getter of some resources, that are shared
// by actions (UrlTranslator).
//------------------------------------------------------------------------------
#pragma once
#include <mutex>
#include <string>
#include <unordered_set>

#include "IAction.h"
#include "CUrlTranslator.h"
#include "CActionQueue.h"
#include "CCompositeDecider.h"

/// @brief Implements how various actions trigger each other.
/// See mediator design pattern.
///
/// Provides 'OnFinished' callback for each type of action. Also provides getter
/// of some resources, that are shared by actions (UrlTranslator).
class CActionMediator
{
public:
    /// @param[in, out] taskQueue Reference to queue of tasks. Mediator might
    /// append new actions to queue every time some action finishes.
    /// @param[in] maxRecursion Recursion limit of HTML links.
    /// @param[in] outDir Directory where mirror should be saved.
    /// @param[in] urlDecider Usecase-specific composite of URL deciders to be
    /// used.
    /// @param[in] leafUrlsToLocalFile Whether to link leaf urls to not-existing
    /// file (true) or whether to link them to the original destination (false).
    /// @param[in] downloadImages Whether to downloadImages (true) or whether to
    /// keep them pointing to the original location.
    CActionMediator(CActionQueue& taskQueue, size_t maxRecursion,
                    const std::string& outDir, CCompositeDecider&& urlDecider,
                    bool leafUrlsToLocalFile, bool downloadImages);

    /// @brief Is invoked after download action is finished.
    ///
    /// @param[in, out] pAction Pointer to finished action.
    void OnDownloadFinished(IAction* pAction);

    /// @brief Is invoked after processing action is finished.
    ///
    /// @param[in, out] pAction Pointer to finished action.
    void OnProcessingFinished(IAction* pAction);

    /// @brief Is invoked after HTML processing action is finished.
    ///
    /// @param[in, out] pAction Pointer to finished action.
    void OnHtmlProcessingFinished(IAction* pAction);

    /// @brief Is invoked after saving action is finished.
    ///
    /// @param[in, out] pAction Pointer to finished action.
    void OnSavingFinished(IAction* pAction);

    /// @brief Is invoked after HTTP parsing action is finished.
    ///
    /// @param[in, out] pAction Pointer to finished action.
    void OnHttpParsingFinished(IAction* pAction);

    /// @brief Shared URL translator getter.
    ///
    /// @return Shared URL translator.
    std::shared_ptr<CUrlTranslator> GetUrlTranslator();

    /// @brief Thread safe way of enqueing actions.
    ///
    /// @param[in] pAction Action to be enqueued.
    void EnqueueAction(SIAction_t pAction);

private:
    /// @brief Thread safe queue of actions.
    CActionQueue& m_taskQueue;

    /// @brief Maximal depth of recursion in downloading HTML documents.
    size_t m_maxRecursion;

    /// @brief Shared URL translator.
    std::shared_ptr<CUrlTranslator> m_pTranslator;

    /// @brief Set of URLs that were already downloaded this run, to prevent
    /// repeated downloads of same page.
    std::unordered_set<std::string> m_downloadedUrls;

    /// @brief Mutex guarding access to m_downloadedUrls.
    std::mutex m_downloadedUrlsMutex;

    /// @brief Usecase-specific composite of URL deciders to be used.
    CCompositeDecider m_urlDecider;

    /// @brief Whether to link leaf urls to not-existing file (true) or whether
    /// to link them to the original destination (false).
    bool m_leafUrlsToLocalFile;

    /// @brief Whether to downloadImages (true) or whether to keep them pointing
    /// to the original location.
    bool m_downloadImages;
};
