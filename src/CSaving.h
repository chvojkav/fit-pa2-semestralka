//------------------------------------------------------------------------------
// File: Saving.h
// Author: Chvojka Vojtech, CTU Prague
// Provides implementation of action that saves data to file.
//------------------------------------------------------------------------------
#pragma once
#include <string>

#include "CAction.h"

/// @brief Action that saves data to m_metadata->m_filename.
class CSaving : public CAction
{
public:
    /// @param[in] data Data to be saved.
    CSaving(std::string&& data);

    /// @see IAction.
    virtual void Run() override;

    /// @see IAction.
    virtual void OnActionFinish() override;

    /// @see IAction.
    virtual void PrintSelf(std::ostream& os) const override;

    /// @brief Data to be saved.
    std::string m_data;
};
