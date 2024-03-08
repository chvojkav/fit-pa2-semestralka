//------------------------------------------------------------------------------
// File: IAction.h
// Author: Chvojka Vojtech, CTU Prague
// Provides an interface to an Action - general worker thread task.
//------------------------------------------------------------------------------
#pragma once
#include <stdexcept>

/// @brief Actions throw this exception if they want to prevent invocation of 
/// OnFinish callback. Error will be automatically logged.
class CActionError : public std::runtime_error
{
public:
    /// @param[in] __arg Error message.
    explicit CActionError(const std::string& __arg) : std::runtime_error(__arg)
    {}
};

//------------------------------------------------------------------------------
/// @brief Interface to an action.
//------------------------------------------------------------------------------
class IAction
{
public:
    virtual ~IAction() = default;

    /// @brief Calls Run and if no exception is thrown, calls OnActionFinish().
    /// Also performs logging.
    void PerformAction();

protected:
    /// @brief Performs an action.
    ///
    /// @throw CActionError If OnActionFinish should not be called.
    virtual void Run() = 0;

    /// @brief Messaging mechanism. Application provides implementation of this
    /// function for all actions it wants to react to.
    virtual void OnActionFinish() = 0;

    /// @brief Prints the action to given ostream.
    /// @param[in, out] os Stream to print to.
    virtual void PrintSelf(std::ostream& os) const = 0;
    friend std::ostream& operator<<(std::ostream& os, const IAction* pAction);

    /// @brief Action priority.
    int m_priority = 0;
};

std::ostream& operator<<(std::ostream& os, const IAction* pAction);
