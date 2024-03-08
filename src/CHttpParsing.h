//------------------------------------------------------------------------------
// File: CHttpParsing.h
// Author: Chvojka Vojtech, CTU Prague
// Implements action that parses http.
//------------------------------------------------------------------------------
#pragma once
#include <string>

#include "CAction.h"

/// @brief Parses HTTP document, throws CActionError if HTTP status was not 200.
class CHttpParsing : public CAction
{
public:
    /// @param[in] http HTTP document to be parsed.
    CHttpParsing(const std::string& http);

    /// @see IAction.
    virtual void Run() override;

    /// @see IAction.
    virtual void OnActionFinish() override;

    /// @see IAction.
    virtual void PrintSelf(std::ostream& os) const override;

    /// @brief There will be stored HTTP data (without headers).
    std::string m_data;

    /// @brief There will be stored type of HTTP data (Content-Type header).
    std::string m_contentType;
};
