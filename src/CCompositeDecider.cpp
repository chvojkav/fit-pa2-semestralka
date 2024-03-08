//------------------------------------------------------------------------------
// File: CCompositeDecider.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include "CCompositeDecider.h"

using namespace std;

bool CCompositeDecider::DecideUrl(const char* pUrl)
{
    for (auto& pDecider : m_deciders)
    {
        if (!pDecider->DecideUrl(pUrl))
        {
            return false;
        }
    }

    return true;
}

void CCompositeDecider::AddDecider(SIUrlDecider_t pDecider)
{
    m_deciders.push_back(pDecider);
}
