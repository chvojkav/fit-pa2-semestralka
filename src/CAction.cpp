//------------------------------------------------------------------------------
// File: CAction.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include "CAction.h"

void CAction::AssignMediator(CActionMediator* pMediator)
{
    m_mediator = pMediator;
}

void CAction::AssignMetadata(const CActionMetadata& metadata)
{
    m_metadata = metadata;
    m_priority = metadata.m_recursionDepth;
}
