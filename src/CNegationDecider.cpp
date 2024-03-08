//------------------------------------------------------------------------------
// File: CNegationDecider.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include "CNegationDecider.h"

CNegationDecider::CNegationDecider(SIUrlDecider_t pUrlDecider)
    : m_pUrlDecider(pUrlDecider)
{}

bool CNegationDecider::DecideUrl(const char* pUrl)
{
    return !m_pUrlDecider->DecideUrl(pUrl);
}
