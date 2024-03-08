//------------------------------------------------------------------------------
// File: IAction.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include "CLog.h"


#include "IAction.h"

using namespace std;

void IAction::PerformAction()
{
    try
    {
        logdebug("Starting " << this);
        Run();
        OnActionFinish();
        logtrace("Finished " << this);
    }
    catch(const CActionError& e)
    {
        logdebug("Action '" << this << "' interrupted by: '" << e.what()
                 << "'");
    }
}

ostream& operator<<(ostream& os, const IAction* pAction)
{
    os << "Prio: " << pAction->m_priority << " ";
    pAction->PrintSelf(os);
    return os;
}
