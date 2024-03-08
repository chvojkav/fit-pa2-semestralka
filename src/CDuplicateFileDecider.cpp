//------------------------------------------------------------------------------
// File: CDuplicateFileDecider.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include "CDuplicateFileDecider.h"

using namespace std;

CDuplicateFileDecider::CDuplicateFileDecider(const string& outDir)
    : m_urlTranslator(outDir)
{}

bool CDuplicateFileDecider::DecideUrl(const char* pUrl)
{
    filesystem::path filePath = m_urlTranslator.GetAbsouluteFilePath(pUrl);
    return !filesystem::exists(filePath);
}

