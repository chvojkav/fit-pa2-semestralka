//------------------------------------------------------------------------------
// File: CAppParam.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include <algorithm>
#include <stdexcept>

#include "CLog.h"


#include "CAppParam.h"

using namespace std;

CAppParam::CAppParam(string paramName, bool required,
                     optional<string> default_)
    : m_name(move(paramName)), m_required(required), m_default(default_)
{}

CAppParam::CAppParam(string paramName, initializer_list<string> choices,
                     bool required, optional<string> default_)
    : m_name(move(paramName)), m_required(required), m_choices(choices),
      m_default(default_)
{}

ostream& operator<<(ostream& os, const CAppParam& param)
{
    if (!param.m_required)
    {
        os << "[";
    }

    os << param.m_name << " ";
    if (param.m_choices.size() == 0)
    {
        os << "<param-value>";
    }
    else
    {
        os << "{";
        for (const string& choice : param.m_choices)
        {
            os << choice << ", ";
        }

        os << "}";
    }

    if (!param.m_required)
    {
        os << "]";
    }

    return os;
}

CAppParam::operator string() const
{
    ThrowIfNotSet();
    return m_value;
}

/// @brief Values that are convertible to bool. Are in lowercase.
const char* validBoolValuesLowercase[] = { "true", "1", "false", "0" };
CAppParam::operator bool() const
{
    ThrowIfNotSet();
    string valueCpy(m_value);
    transform(valueCpy.begin(), valueCpy.end(), valueCpy.begin(),
                   [](unsigned char c) { return tolower(c); });

    bool ret;
    if (valueCpy == validBoolValuesLowercase[0]
        || valueCpy == validBoolValuesLowercase[1])
    {
        ret = true;
    }
    else if (valueCpy == validBoolValuesLowercase[2]
             || valueCpy == validBoolValuesLowercase[3])
    {
        ret = false;
    }
    else
    {
        THROW(invalid_argument, "Value: '" << m_value
                                << "' is not convertible to bool.");
    }

    return ret;
}

CAppParam::operator int() const
{
    ThrowIfNotSet();
    return stoi(m_value);
}

void CAppParam::ThrowIfNotSet() const
{
    if (!m_isSet)
    {
        THROW(invalid_argument, "Parameter is not set: '" << m_name << "'.");
    }
}
