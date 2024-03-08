//------------------------------------------------------------------------------
// File: CActionQueue.h
// Author: Chvojka Vojtech, CTU Prague
// Thread safe queue of Actions. Threads are both producer and consumer, so once
// all threads are waiting, automatically signals end to itself.
//------------------------------------------------------------------------------
#pragma once
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>

#include "IAction.h"


/// @brief Thrown once queue would block indefinetly.
class CQueueError : public std::runtime_error
{
public:
    /// @param[in] __arg Description of error.
    explicit CQueueError(const std::string& __arg) : std::runtime_error(__arg)
    {}
};

/// @brief Shared pointer to action interface type.
typedef std::shared_ptr<IAction> SIAction_t;
/// @brief Queue of SIAction_t type.
typedef std::queue<SIAction_t> TaskQueue_t;

/// @brief Queue of actions with thread-safe interface.
class CActionQueue
{
public:
    /// @param[in] maxWaiterCnt Queue will automatically trigger SignalEnd()
    /// once there are this count of waiting threads. Set this to SIZE_MAX if
    /// you plan on signaling end manualy.
    CActionQueue(size_t maxWaiterCnt);

    /// @brief Thread-safe way of pushing action to queue.
    ///
    /// @param[in] pAction Pointer to action to be appended.
    void EnquqeAction(SIAction_t pAction);

    /// @brief Thread-safe way of popping action from queue.
    ///
    /// @throw CQueueError End was signaled.
    ///
    /// @return Action popped from queue.
    SIAction_t WaitForAction();

    /// @brief Signals end to all threads waiting for an action and prevents
    /// any further popping from the queue.
    void SignalEnd();

private:
    /// @brief The internal queue representation.
    TaskQueue_t m_queue;

    /// @brief Mutex guarding m_queue.
    std::mutex m_queueMutex;

    /// @brief Condition variable on which threads wait when no action is present.
    std::condition_variable m_waitable;

    /// @brief Queue will automatically trigger SignalEnd() once there are this
    /// count of waiting threads. Set this to SIZE_MAX if you plan on signaling
    /// end manualy
    size_t m_maxWaiterCnt;

    /// @brief Current count of threads that wait for an action.
    size_t m_currentWaiterCnt = 0;

    /// @brief Flag whether the end was signaled.
    bool m_endSignaled = false;
};
