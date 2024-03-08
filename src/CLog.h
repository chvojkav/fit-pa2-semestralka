//------------------------------------------------------------------------------
// File: Log.h
// Author: Chvojka Vojtech, CTU Prague
// Provides thread-safe logging mechanism, that also supports:
//     - Severity levels
//     - Convenience macros that enable message composition using operator <<.
//     - THROW macro that prepends [FILE:LINE] to error message and enables
//       message composition using operator <<.
// Does not support:
//     - Dynamic severity level change.
//------------------------------------------------------------------------------
#pragma once
#include <iostream>
#include <mutex>
#include <string>
#include <sstream>

#define LOG_LVL_TRACE 0
#define LOG_LVL_DEBUG 1
#define LOG_LVL_INFO 2
#define LOG_LVL_WARNING 3
#define LOG_LVL_ERROR 4
#define LOG_LVL_FATAL 5
#define LOG_LVL_NO_LOG 6

#define LOG_LVL LOG_LVL_DEBUG

#if (LOG_LVL_ERROR >= LOG_LVL)
#define logerror(expression) { \
    std::stringstream s; \
    s << expression; \
    CLogger::Log("ERROR", s.str()); \
}
#else // #if (LOG_LVL_ERROR >= LOG_LVL)
#define logerror(expression) (void)0
#endif // #if (LOG_LVL_ERROR >= LOG_LVL)

#if (LOG_LVL_INFO >= LOG_LVL)
#define loginfo(expression) { \
    std::stringstream s; \
    s << expression; \
    CLogger::Log("INFO ", s.str()); \
}
#else // #if (LOG_LVL_INFO >= LOG_LVL)
#define loginfo(expression) (void)0
#endif // #if (LOG_LVL_INFO >= LOG_LVL)

#if (LOG_LVL_DEBUG >= LOG_LVL)
#define logdebug(expression) { \
    std::stringstream s; \
    s << expression; \
    CLogger::Log("DEBUG", s.str()); \
}
#else // #if (LOG_LVL_DEBUG >= LOG_LVL)
#define logdebug(expression) (void)0
#endif // #if (LOG_LVL_DEBUG >= LOG_LVL)

#if (LOG_LVL_TRACE >= LOG_LVL)
#define logtrace(expression) { \
    std::stringstream s; \
    s << expression; \
    CLogger::Log("TRACE", s.str()); \
}
#else // #if (LOG_LVL_DEBUG >= LOG_LVL)
#define logtrace(expression) (void)0
#endif // #if (LOG_LVL_DEBUG >= LOG_LVL)

//-------------------------------------------------------------------------------
/// @brief Uniform thread-safe logging interface.
//-------------------------------------------------------------------------------
class CLogger
{
public:
    /// @brief Thread sage log.
    /// @param[in] pLevel Severity level identifier.
    /// @param[in] message Message to be printed.
    inline static void Log(const char* pLevel, const std::string& message)
    {
        std::lock_guard<std::mutex> consoleGuard(CLogger::m_consoleMutex);
        std::cout << "[" << pLevel << "]: " << message << std::endl;
    }
private:
    CLogger() = default;

    /// @brief Mutex protecting std::cout.
    static std::mutex m_consoleMutex;
};

/// @brief Throws an exception and prepends error message with [FILE:LINE]
///
/// @param[in] exceptionType Type of exception to be thrown.
/// @param[in] expression Stream-like (os << ...) error message.
#define THROW(exceptionType, expression) { \
    std::stringstream s; \
    s << '[' << __FILE__ << ':' << __LINE__ << "]: " << expression; \
    throw exceptionType(s.str()); \
}


