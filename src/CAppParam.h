//------------------------------------------------------------------------------
// File: CAppParam.h
// Author: Chvojka Vojtech, CTU Prague
// Represents comand line parameter.
//------------------------------------------------------------------------------
#pragma once
#include <string>
#include <vector>
#include <optional>

/// @brief Represents comand line parameter. Allows client to define, whether
/// the parameter is required/optional, set its default value, and limit allowed
/// values (choices).
struct CAppParam
{
    /// @param[in] paramName Name of parameter.
    /// @param[in] required Whether the parameter is required.
    /// @param[in] default_ Default value of parameter.
    CAppParam(std::string paramName, bool required = false,
              std::optional<std::string> default_ = std::nullopt);

    /// @param[in] paramName Name of parameter.
    /// @param[in] choices Allowed values.
    /// @param[in] required Whether the parameter is required.
    /// @param[in] default_ Default value of parameter.
    CAppParam(std::string paramName,
              std::initializer_list<std::string> choices,
              bool required = false,
              std::optional<std::string> default_ = std::nullopt);

    /// @brief Getter of parameter value as std::string.
    ///
    /// @return std::string representation of parameter value.
    explicit operator std::string() const;

    /// @brief Getter of parameter value as bool.
    ///
    /// @return bool representation of parameter value.
    explicit operator bool() const;

    /// @brief Getter of parameter value as integer.
    ///
    /// @return integer representation of parameter value.
    explicit operator int() const;

    /// @brief Name of parameter.
    std::string m_name;

    /// @brief Value of parameter.
    std::string m_value;

    /// @brief Whether the parameter is required.
    bool m_required;

    /// @brief Whether the parameter is set.
    bool m_isSet = false;

    /// @brief Allowed values of parameter value.
    std::vector<std::string> m_choices;

    /// @brief Default value of parameter.
    std::optional<std::string> m_default;

private:
    /// @brief Checks whether the parameter is set and if it's not throws an 
    /// exception with nicely formatted message.
    ///
    /// @throw invalid_argument Parameter is not set.
    void ThrowIfNotSet() const;
};

/// @brief Pretty prints the parameter.
std::ostream& operator<<(std::ostream& os, const CAppParam& param);
