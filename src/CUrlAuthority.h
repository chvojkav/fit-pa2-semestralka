//------------------------------------------------------------------------------
// File: CUrlAuthority.h
// Author: Chvojka Vojtech, CTU Prague
// Utility for parsing and composition of authority part of URL.
//------------------------------------------------------------------------------
#pragma once
#include <string>

/// @brief Utility class for parsing and composing authority part of URL.
struct CUrlAuthority
{
    CUrlAuthority() = default;

    /// @param[in] auth Authority. f.e. '//chvojvoj:pswd@fit.cvut.cz:22'
    CUrlAuthority(const std::string& auth);

    /// @brief Whether URL authority has user info part.
    ///
    /// @retval true Has user info.
    /// @retval false Does not have user info.
    bool hasUserInfo() const;

    /// @brief Whether URL authority has port part.
    ///
    /// @retval true Has port.
    /// @retval false Does not have port.
    bool hasPort() const;

    /// @brief Composes URL authority from its members.
    ///
    /// @return Composed URL authority.
    explicit operator std::string() const;

    /// @brief User info part of URL.
    std::string userinfo;

    /// @brief Host part of URL.
    std::string host;

    /// @brief Port part of URL.
    std::string port;
};
