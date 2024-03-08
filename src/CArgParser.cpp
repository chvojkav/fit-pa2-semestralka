//------------------------------------------------------------------------------
// File: CArgParser.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include <algorithm>
#include <sstream>
#include <string>

#include "CLog.h"


#include "CArgParser.h"

using namespace std;

/// @brief Sets string to expression it the string is empty.
/// 
/// @param[in, out] _str String to be checked for emptiness and eventually set.
/// @param[in] expression Stream-like expression (os << ...) to be set into string.
#define SET_IF_EMPTY(_str, expression) { \
    if (_str.empty()) \
    { \
        stringstream s; \
        s << expression; \
        _str = s.str(); \
    } \
}

uint16_t CArgParser::ParseArgs(int argc, const char** argv)
{
    string binaryName = argv[0];
    string firstEncounteredError;
    argv++;
    argc--;

    uint16_t parsedCnt = 0;

    bool printHelp = false;
    bool parsingOk = true;
    while (argc > 0)
    {
        if (argv[0] == string("--help"))
        {
            printHelp = true;
            argv++;
            argc--;
            continue;
        }

        parsingOk = false;
        bool matched = false;
        for (auto& param_it : params)
        {
            CAppParam& param = param_it.second;
            if (param.m_name == argv[0])
            {
                // Parameter name is same as current parameter.
                if (param.m_isSet)
                {
                    SET_IF_EMPTY(firstEncounteredError,
                                 "Duplicate argument: '" << argv[0] << "'.");
                    printHelp = true;
                    argc = 0; // This breaks from parsing.
                }
                else if (argc < 2)
                {
                    SET_IF_EMPTY(firstEncounteredError,
                                 "Missing argument value: '" << argv[0] << "'.");
                    printHelp = true;
                    argc = 0; // This breaks from parsing.
                }
                else
                {
                    argv++;
                    argc--;
                    param.m_value = argv[0];
                    param.m_isSet = true;
                    argv++;
                    argc--;
                    matched = true;
                    parsedCnt++;
                }

                break;
            }
        } // for

        if (!matched)
        {
            SET_IF_EMPTY(firstEncounteredError,
                         "Error while parsing argument: '" << argv[0] << "'.");
            break;
        }

        parsingOk = true;
    } // while

    if (parsingOk)
    {
        // Fill in default values.
        for (auto& param_it : params)
        {
            CAppParam& param = param_it.second;
            if (!param.m_isSet && param.m_default.has_value())
            {
                param.m_value = *param.m_default;
                param.m_isSet = true;
            }
        }
    }

    if (parsingOk)
    {
        // Check required arguments.
        for (const auto& param_it : params)
        {
            const CAppParam& param = param_it.second;
            if (param.m_required && !param.m_isSet)
            {
                SET_IF_EMPTY(firstEncounteredError,
                             "Missing required argument: '" << param.m_name << "'.");
                printHelp = true;
                parsingOk = false;
                break;
            }
        }
    }

    if (parsingOk)
    {
        // Check choices.
        for (const auto& param_it : params)
        {
            const CAppParam& param = param_it.second;
            if (param.m_isSet
                && param.m_choices.size() > 0
                && find(param.m_choices.begin(),
                             param.m_choices.end(),
                             param.m_value) == param.m_choices.end())
            {
                stringstream msg;
                msg << "Value '" << param.m_value << "' of parameter '"
                    << param.m_name << "' is not from choice list: (";
                for (const string& choice : param.m_choices)
                {
                    msg << "'" << choice << "', ";
                }

                msg << ").";

                SET_IF_EMPTY(firstEncounteredError, msg.str());
                printHelp = true;
                parsingOk = false;
                break;
            }
        }
    }

    if (printHelp)
    {
        stringstream help;
        help << "Usage:\n" << binaryName << "\n";
        help << "\t[--help]\n";
        for (const auto param_it : params)
        {
            const CAppParam& param = param_it.second;
            help << "\t" << param << "\n";
        }

        loginfo(help.str());
    }

    if (!parsingOk)
    {
        throw invalid_argument(firstEncounteredError);
    }

    return parsedCnt;
}

void CArgParser::Add(const CAppParam& param)
{
    params.insert({param.m_name, param});
}

const CAppParam& CArgParser::Get(const string& name) const
{
    return params.at(name);
}

const CAppParam& CArgParser::operator[](const string& name) const
{
    return params.at(name);
}
