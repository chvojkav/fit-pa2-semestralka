//------------------------------------------------------------------------------
// File: CArgParser.h
// Author: Chvojka Vojtech, CTU Prague
// Utility for parsing command line arguments.
//------------------------------------------------------------------------------
#pragma once
#include <string>
#include <unordered_map>

#include "CAppParam.h"

// TODO support for format "--<arg-name>=<arg-value>"

/// @brief Parser of command line arguments.
class CArgParser
{
public:
    /// @brief Parses command line arguments that were defined with Add().
    ///
    /// @param[in] argc Count of arguments.
    /// @param[in] argv Array of c-string arguments.
    /// @return Count of parsed arguments.
    uint16_t ParseArgs(int argc, const char** argv);

    /// @brief Adds parameter definition to parser.
    ///
    /// @param[in] param Parameter definition.
    void Add(const CAppParam& param);

    /// @brief Getter of parameter by name.
    ///
    /// @param[in] name Name of parameter
    /// @throw std::out_of_range If no such parameter is present.
    /// @return Found parameter definition.
    const CAppParam& Get(const std::string& name) const;

    /// @brief Getter of parameter by name.
    ///
    /// @param[in] name Name of parameter
    /// @throw std::out_of_range If no such parameter is present.
    /// @return Found parameter definition.
    const CAppParam& operator[](const std::string& name) const;
private:
    /// @brief Map of defined parameters.
    std::unordered_map<std::string, CAppParam> params;
};
