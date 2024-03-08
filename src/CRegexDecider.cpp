//------------------------------------------------------------------------------
// File: CRegexDecider.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include "CRegexDecider.h"

using namespace std;

CRegexDecider::CRegexDecider(const string& regex)
    : m_regex(regex)
{}

bool CRegexDecider::DecideUrl(const char* pUrl)
{
    return regex_match(pUrl, m_regex);
}
