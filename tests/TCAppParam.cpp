#include "catch.hpp"
#include "../src/CAppParam.h"


TEST_CASE("CAppParam")
{
    SECTION("Conversion throws if not set.")
    {
        CAppParam param("--test");
        REQUIRE_THROWS_AS((bool)param, std::invalid_argument);
        REQUIRE_THROWS_AS((int)param, std::invalid_argument);
        REQUIRE_THROWS_AS((std::string)param, std::invalid_argument);
    }

    SECTION("Conversions.")
    {
        CAppParam param("--test");
        param.m_value = "1";
        param.m_isSet = true;
        REQUIRE((bool)param == true);
        REQUIRE((int)param == 1);
        REQUIRE((std::string)param == "1");
    }

    SECTION("Boolean conversion.")
    {
        CAppParam param("--test");
        param.m_isSet = true;
        param.m_value = "1";
        REQUIRE((bool)param == true);
        param.m_value = "true";
        REQUIRE((bool)param == true);
        param.m_value = "TRUE";
        REQUIRE((bool)param == true);
        param.m_value = "TrUe";
        REQUIRE((bool)param == true);
        param.m_value = "0";
        REQUIRE((bool)param == false);
        param.m_value = "false";
        REQUIRE((bool)param == false);
        param.m_value = "FALSE";
        REQUIRE((bool)param == false);
        param.m_value = "FaLsE";
        REQUIRE((bool)param == false);
        param.m_value = "wrong value";
        REQUIRE_THROWS_AS((bool)param, std::invalid_argument);
    }
}