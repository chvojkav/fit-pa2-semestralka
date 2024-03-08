//------------------------------------------------------------------------------
// File: CHtmlProcessing.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include <libxml/parser.h>

#include "CUrlTranslator.h"


#include "CHtmlProcessing.h"

using namespace std;

CHtmlProcessing::CHtmlProcessing(const string& html,
                                 const shared_ptr<IUrlDecider>& pDecider,
                                 bool leafUrlsToLocalFile,
                                 bool downloadImages)
    : CProcessing(html), m_pDecider(pDecider),
      m_leafUrlsToLocalFile(leafUrlsToLocalFile),
      m_downloadImages(downloadImages)
{}

/// @brief Walks XML tree, searches for URLs and replaces them with
/// absolute-URLS/File path.
///
/// @param[in, out] pNode Pointer to root node of XML tree.
/// @param[out] foundLinks Any link that was found and enabled by decider will
/// be added here
/// @param[in, out] pDecider Pointer to URL decider to be used to decide URLs.
/// @param[in] pTranslator Pointer to URL translator to be used to translate URLs
/// to absolute URLs and file paths.
/// @param[in] rootUrl Root URL for converting absolute/relative URLs.
/// @param[in] leafUrlsToLocalFile Whether to link leaf urls to not-existing file.
/// @param[in] downloadImages Whether to downloadImages.
static void _RecursivelyReplaceUrls(xmlNodePtr pNode,
                                   vector<string>& foundLinks,
                                   IUrlDecider* pDecider,
                                   CUrlTranslator* pTranslator,
                                   const string& rootUrl,
                                   const bool& leafUrlsToLocalFile,
                                   const bool& downloadImages);

void CHtmlProcessing::Process()
{
    string& html = m_rawData;
    xmlDocPtr pDoc = xmlReadMemory(html.c_str(), html.length(), NULL, NULL,
                                   XML_PARSE_RECOVER | XML_PARSE_NOERROR
                                   | XML_PARSE_NOWARNING);
    xmlNodePtr node = xmlDocGetRootElement(pDoc);
    _RecursivelyReplaceUrls(node, m_foundLinks, m_pDecider.get(),
                           m_mediator->GetUrlTranslator().get(),
                           m_metadata->m_url, m_leafUrlsToLocalFile,
                           m_downloadImages);

    xmlChar* pDumpedXml = nullptr;
    int size = 0;
    xmlDocDumpMemory(pDoc, &pDumpedXml, &size);

    m_processedData = (char*)pDumpedXml;
    xmlFree(pDumpedXml);
    xmlFreeDoc(pDoc);
}

void CHtmlProcessing::OnActionFinish()
{
    m_mediator->OnHtmlProcessingFinished(this);
}

void CHtmlProcessing::PrintSelf(ostream& os) const
{
    os << "HTML-processing of " << m_metadata->m_url;
}

static void _RecursivelyReplaceUrls(xmlNodePtr pNode,
                                   vector<string>& foundLinks,
                                   IUrlDecider* pDecider,
                                   CUrlTranslator* pTranslator,
                                   const string& rootUrl,
                                   const bool& leafUrlsToLocalFile,
                                   const bool& downloadImages)
{
    xmlNodePtr pTmp = pNode;
    for (; pTmp != NULL; pTmp = pTmp->next)
    {
        xmlChar* url = xmlGetProp(pTmp, BAD_CAST "href");
        if (url != NULL)
        {
            string absoluteUrl = pTranslator->GetAbsoluteUrl(rootUrl,
                                                                  (char*)url);
            if (pDecider->DecideUrl(absoluteUrl.c_str()))
            {
                foundLinks.push_back(absoluteUrl);
                string relativePath = 
                    pTranslator->GetRelativeFilePath(rootUrl, absoluteUrl);
                xmlSetProp(pTmp, BAD_CAST "href", (xmlChar*)relativePath.c_str());
            }
            else
            {
                if (leafUrlsToLocalFile)
                {
                    xmlSetProp(pTmp, BAD_CAST "href",
                               (xmlChar*)"thisfilesuredoesnotexist-qweus.html");
                }
                else
                {
                    xmlSetProp(pTmp, BAD_CAST "href", (xmlChar*)absoluteUrl.c_str());
                }
            }
        }
        xmlFree(url);

        if (string("img") == (char*)pTmp->name)
        {
            xmlChar* url = xmlGetProp(pTmp, BAD_CAST "src");
            if (url != NULL)
            {
                string absoluteUrl = pTranslator->GetAbsoluteUrl(rootUrl,
                                                                    (char*)url);
                if (pDecider->DecideUrl(absoluteUrl.c_str())
                    && downloadImages)
                {
                    foundLinks.push_back(absoluteUrl);
                    string relativePath = 
                        pTranslator->GetRelativeFilePath(rootUrl, absoluteUrl);
                    xmlSetProp(pTmp, BAD_CAST "src", (xmlChar*)relativePath.c_str());
                }
                else
                {
                    xmlSetProp(pTmp, BAD_CAST "src", (xmlChar*)absoluteUrl.c_str());
                }
            }
            xmlFree(url);
        }

        _RecursivelyReplaceUrls(pTmp->children, foundLinks, pDecider,
                               pTranslator, rootUrl, leafUrlsToLocalFile,
                               downloadImages);
    }
}
