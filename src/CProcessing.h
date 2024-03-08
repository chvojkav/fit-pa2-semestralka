//------------------------------------------------------------------------------
// File: Processing.h
// Author: Chvojka Vojtech, CTU Prague
// Action that represents raw-data processing.
//------------------------------------------------------------------------------
#pragma once
#include <string>

#include "CAction.h"

//------------------------------------------------------------------------------
/// @brief Represents chunk of data that can be processed.
//------------------------------------------------------------------------------
class CProcessing : public CAction
{
public:
    CProcessing(const std::string& rawData);

    /// @brief Processes m_rawData and moves processed data to m_processedData.
    /// By default (no override) no-op.
    virtual void Process();

    /// @brief Translates URL to filename and calls Process().
    /// @see IAction.
    virtual void Run() override final;

    /// @see IAction.
    virtual void OnActionFinish() override;

    /// @see IAction.
    virtual void PrintSelf(std::ostream& os) const override;

    /// @brief Raw data to be processed.
    std::string m_rawData;

    /// @brief Processed data.
    std::string m_processedData;
};
