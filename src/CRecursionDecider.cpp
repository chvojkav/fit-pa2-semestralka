//------------------------------------------------------------------------------
// File: CRecursionDecider.h
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include "CRecursionDecider.h"

CRecursionDecider::CRecursionDecider(size_t recursionDepth, size_t allowedDepth)
    : m_recursionDepth(recursionDepth), m_allowedDepth(allowedDepth)
{}

bool CRecursionDecider::DecideUrl(const char* pUnused)
{
    (void)pUnused;
    return m_recursionDepth < m_allowedDepth;
}
