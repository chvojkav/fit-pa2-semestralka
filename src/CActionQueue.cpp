//------------------------------------------------------------------------------
// File: CActionQueue.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include "CLog.h"


#include "CActionQueue.h"

using namespace std;

CActionQueue::CActionQueue(size_t maxWaiterCnt)
    : m_maxWaiterCnt(maxWaiterCnt)
{}

void CActionQueue::EnquqeAction(SIAction_t pAction)
{
    {
        // Critical section entered.
        lock_guard<mutex> queueGuard(m_queueMutex);
        m_queue.push(pAction);
    }
    // Critical section left.

    m_waitable.notify_one();
}

SIAction_t CActionQueue::WaitForAction()
{
    unique_lock<mutex> lck(m_queueMutex);
    // Critical section entered.

    if (!m_queue.empty())
    {
        SIAction_t ret = m_queue.front();
        m_queue.pop();
        return ret;
    }

    m_currentWaiterCnt++;
    if (m_currentWaiterCnt >= m_maxWaiterCnt)
    {
        // All worker threads are waiting. Since all threads are both producer
        // and consumer, no more data will come -> signal end.
        // It's okay to unlock, since we're going to signal end and exit funtion
        // with throw.
        lck.unlock();
        SignalEnd();
    }

    if (m_endSignaled)
    {
        THROW(CQueueError, "End signaled.");
    }

    while (true)
    {
        // Critical section left.
        m_waitable.wait(lck);
        // Critical section entered.

        if (m_endSignaled)
        {
            THROW(CQueueError, "End signaled.");
        }

        if (!m_queue.empty())
        {
            break;
        }
    }

    m_currentWaiterCnt--;
    SIAction_t ret = m_queue.front();
    m_queue.pop();
    return ret;
    // Critical section left.
}

void CActionQueue::SignalEnd()
{
    {
        lock_guard<mutex> queueGuard(m_queueMutex);
        // Critical section entered.
        m_endSignaled = true;
    }
    // Critical section left.

    m_waitable.notify_all();
}
