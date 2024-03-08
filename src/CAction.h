//------------------------------------------------------------------------------
// File: CAction.h
// Author: Chvojka Vojtech, CTU Prague
// Provides implementation of assigning mediator and metadata.
//------------------------------------------------------------------------------
#pragma once
#include <optional>

#include "IAction.h"
#include "CActionMediator.h"
#include "CActionMetadata.h"

/// @brief Provides implementation of assigning mediator and metadata.
class CAction : public IAction
{
public:
    /// @brief Assigns mediator to action.
    ///
    /// @param[in] pMediator Pointer to mediator.
    void AssignMediator(CActionMediator* pMediator);

    /// @brief Assigns metadata to action.
    ///
    /// @param[in] metadata Metadata to be assigned.
    void AssignMetadata(const CActionMetadata& metadata);

    virtual ~CAction() = default;

    /// @brief Action metadata.
    std::optional<CActionMetadata> m_metadata;
protected:
    /// @brief Pointer to action mediator.
    CActionMediator* m_mediator = nullptr;
};
