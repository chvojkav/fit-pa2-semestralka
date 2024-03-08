//------------------------------------------------------------------------------
// File: CActionMediator.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include <memory>

#include "CDownloading.h"
#include "CHtmlProcessing.h"
#include "CHttpParsing.h"
#include "CLog.h"
#include "CProcessing.h"
#include "CRecursionDecider.h"
#include "CSaving.h"


#include "CActionMediator.h"

using namespace std;

/// @brief Enables debugging of mediator messaging system. If defined, logs
/// error message for each 'OnFinished' callback called from unexpected type.
#define WARN_ABOUT_WRONG_ACTION_TYPE

#if defined(WARN_ABOUT_WRONG_ACTION_TYPE)
    #include <typeinfo>
    /// @brief Macro appended after 'if (pDynamicallyCastedAction != nullptr){}'
    /// It logs message if cast wasn't possible.
    #define WARN_WRONG_ACTION_TYPE(pAction) \
    else \
    { \
        logerror("Wrong action type: " << typeid(pAction).name()); \
    }
#else
    #define WARN_WRONG_ACTION_TYPE(pAction) (void)0;
#endif // #if defined(WARN_ABOUT_WRONG_ACTION_TYPE)

/// @brief Convenience function that prevents code duplication. Assigns metadata
/// to action, assigns mediator to action, logs 'enqueing of action' and then
/// enqueues and action to mediator.
///
/// @param[in, out] pAction Pointer to action to be enqueued.
/// @param[in] metadata Metadata to be assigned.
/// @param[in, out] pMediator Pointer to mediator to be assigned,
static inline void _EnqueueAction(shared_ptr<CAction> pAction,
                                  const CActionMetadata& metadata,
                                  CActionMediator* pMediator);

//------------------------------------------------------------------------------
// CActionMediator definitions follow:

CActionMediator::CActionMediator(CActionQueue& taskQueue, size_t maxRecursion,
                                 const string& outDir,
                                 CCompositeDecider&& urlDecider,
                                 bool leafUrlsToLocalFile,
                                 bool downloadImages)
    : m_taskQueue(taskQueue), m_maxRecursion(maxRecursion),
      m_pTranslator(make_shared<CUrlTranslator>(outDir)),
      m_urlDecider(urlDecider), m_leafUrlsToLocalFile(leafUrlsToLocalFile),
      m_downloadImages(downloadImages)
{}

void CActionMediator::OnDownloadFinished(IAction* pAction)
{
    CDownloading* pDownload = dynamic_cast<CDownloading*>(pAction);
    if (pDownload != nullptr)
    {
        // Data will be further used as a c-string. Appending terminating zero
        // to be safe for sure.
        pDownload->m_data.push_back('\0');

        string data((char*)pDownload->m_data.data(), pDownload->m_data.size());
        shared_ptr<CHttpParsing> pHttpParsing = make_shared<CHttpParsing>(data);

        _EnqueueAction(pHttpParsing, *pDownload->m_metadata, this);
    }
    WARN_WRONG_ACTION_TYPE(pAction)
}

void CActionMediator::OnHttpParsingFinished(IAction* pAction)
{
    CHttpParsing* pHttpParsing = dynamic_cast<CHttpParsing*>(pAction);
    if (pHttpParsing != nullptr)
    {
        shared_ptr<CAction> pNewAction;
        if (pHttpParsing->m_contentType.rfind("text/html") != string::npos)
        {
            shared_ptr<CCompositeDecider> pDecider =
                make_shared<CCompositeDecider>(m_urlDecider);

            pDecider->AddDecider(make_shared<CRecursionDecider>(
                                     pHttpParsing->m_metadata->m_recursionDepth,
                                     m_maxRecursion));

            pNewAction = make_shared<CHtmlProcessing>(move(pHttpParsing->m_data),
                                                      pDecider,
                                                      m_leafUrlsToLocalFile,
                                                      m_downloadImages);
        }
        else
        {
            pNewAction = make_shared<CProcessing>(move(pHttpParsing->m_data));
        }

        _EnqueueAction(pNewAction, *pHttpParsing->m_metadata, this);
    }
    WARN_WRONG_ACTION_TYPE(pAction)
}

void CActionMediator::OnProcessingFinished(IAction* pAction)
{
    CProcessing* pProcessing = dynamic_cast<CProcessing*>(pAction);
    if (pProcessing != nullptr)
    {
        shared_ptr<CSaving> pSaving =
            make_shared<CSaving>(move(pProcessing->m_processedData));

        _EnqueueAction(pSaving, *pProcessing->m_metadata, this);
    }
    WARN_WRONG_ACTION_TYPE(pAction)
}

void CActionMediator::OnHtmlProcessingFinished(IAction* pAction)
{
    OnProcessingFinished(pAction);

    CHtmlProcessing* pProcessing = dynamic_cast<CHtmlProcessing*>(pAction);
    if (pProcessing != nullptr)
    {
        pProcessing->m_metadata->m_recursionDepth++;

        for (auto& link : pProcessing->m_foundLinks)
        {
            // For each found link decide whether it is already downloaded.
            // If it's not, enqueue new downloading action.

            {
                lock_guard<mutex> urlsGuard(m_downloadedUrlsMutex);
                // Critical section entered.
                if (m_downloadedUrls.find(link) != m_downloadedUrls.end())
                {
                    continue;
                }

                m_downloadedUrls.insert(link);
            }
            // Critical section left.

            shared_ptr<CDownloading> pDownloading =
                make_shared<CDownloading>();
            pProcessing->m_metadata->m_url = link;
            _EnqueueAction(pDownloading, *pProcessing->m_metadata, this);
        }
    }
    WARN_WRONG_ACTION_TYPE(pAction)
}

void CActionMediator::OnSavingFinished(IAction* pAction)
{
    CSaving* pSaving = dynamic_cast<CSaving*>(pAction);
    if (pSaving != nullptr)
    {
        // No-op.
    }
    WARN_WRONG_ACTION_TYPE(pAction)
}

shared_ptr<CUrlTranslator> CActionMediator::GetUrlTranslator()
{
    return m_pTranslator;
}

void CActionMediator::EnqueueAction(SIAction_t pAction)
{
    m_taskQueue.EnquqeAction(pAction);
}

//------------------------------------------------------------------------------
// Static function definition follows:

static inline void _EnqueueAction(shared_ptr<CAction> pAction,
                                  const CActionMetadata& metadata,
                                  CActionMediator* pMediator)
{
    pAction->AssignMetadata(metadata);
    pAction->AssignMediator(pMediator);
    logtrace("Enqueing " << pAction.get());
    pMediator->EnqueueAction(pAction);
}
