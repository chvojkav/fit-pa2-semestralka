#include "catch.hpp"
#include "../src/CArgParser.h"

TEST_CASE("CArgParser")
{
    CArgParser parser;

    SECTION("Not defined argument throws.")
    {
        parser.Add(CAppParam("--some"));
        REQUIRE_THROWS_AS(parser["--other"], std::out_of_range);
        REQUIRE_THROWS_AS(parser.Get("--any"), std::out_of_range);
    }

    SECTION("Missing value throws.")
    {
        parser.Add(CAppParam("--test"));
        int argc = 2;
        const char* argv[] = {"a.out", "--test"};
        REQUIRE_THROWS_AS(parser.ParseArgs(argc, argv), std::invalid_argument);
    }

    SECTION("Missing required throws.")
    {
        parser.Add(CAppParam("--test", true));
        int argc = 1;
        const char* argv[] = {"a.out"};
        REQUIRE_THROWS_AS(parser.ParseArgs(argc, argv), std::invalid_argument);
    }

    SECTION("Unknown parameter throws.")
    {
        parser.Add(CAppParam("--test", true));
        int argc = 3;
        const char* argv[] = {"a.out", "--unknown", "hihi"};
        REQUIRE_THROWS_AS(parser.ParseArgs(argc, argv), std::invalid_argument);
    }

    SECTION("Disallowed value throws.")
    {
        parser.Add(CAppParam("--test", {"allowed-value"}, true));
        int argc = 3;
        const char* argv[] = {"a.out", "--test", "hihi"};
        REQUIRE_THROWS_AS(parser.ParseArgs(argc, argv), std::invalid_argument);
    }

    SECTION("Remaining argument throws.")
    {
        parser.Add(CAppParam("--test", true));
        int argc = 5;
        const char* argv[] = {"a.out", "--test", "hihi", "--remainder", "lol"};
        REQUIRE_THROWS_AS(parser.ParseArgs(argc, argv), std::invalid_argument);
    }

    SECTION("Returns number of parsed arguments")
    {
        parser.Add(CAppParam("--test", true));
        parser.Add(CAppParam("--test2", false));
        int argc = 5;
        const char* argv[] = {"a.out", "--test", "hihi", "--test2", "lol"};
        REQUIRE(parser.ParseArgs(argc, argv) == 2);
    }

    SECTION("Returns number of parsed arguments II")
    {
        parser.Add(CAppParam("--test", true));
        parser.Add(CAppParam("--test2", false));
        int argc = 3;
        const char* argv[] = {"a.out", "--test", "hihi"};
        REQUIRE(parser.ParseArgs(argc, argv) == 1);
    }

    SECTION("Value is correct")
    {
        parser.Add(CAppParam("--test"));
        int argc = 3;
        const char* argv[] = {"a.out", "--test", "random-param-val-v8q0vybqrvbifdc"};
        parser.ParseArgs(argc, argv);
        REQUIRE((std::string)parser["--test"] == "random-param-val-v8q0vybqrvbifdc");
    }
}
