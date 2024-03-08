#include "catch.hpp"
#include "../src/CUrl.h"


TEST_CASE("CUrl")
{
    SECTION("Invalid URL throws.")
    {
        REQUIRE_THROWS_AS(CUrl("httD://why.cz"), std::invalid_argument);
        REQUIRE_THROWS_AS(CUrl("seznam.cz"), std::invalid_argument);
        REQUIRE_THROWS_AS(CUrl("http://ssda*csa.cz"), std::invalid_argument);
        REQUIRE_THROWS_AS(CUrl("http://>>abc.cz"), std::invalid_argument);
        REQUIRE_THROWS_AS(CUrl("https://Oh_My.cz"), std::invalid_argument);
        REQUIRE_THROWS_AS(CUrl("https//seznam.cz"), std::invalid_argument);
        REQUIRE_THROWS_AS(CUrl("https:/seznam.cz"), std::invalid_argument);
        REQUIRE_THROWS_AS(CUrl("https:seznam.cz"), std::invalid_argument);
        REQUIRE_THROWS_AS(CUrl("http:///"), std::invalid_argument);
    }

    SECTION("URL parsed correctly")
    {
        std::string urlString("https://fit.cvut.cz:1234/path/to/resource?query=abc#some-fragment");
        CUrl url(urlString);
        REQUIRE(url.scheme == "https");
        REQUIRE((std::string)url.authority == "//fit.cvut.cz:1234");
        REQUIRE(url.path == "/path/to/resource");
        REQUIRE(url.query == "query=abc");
        REQUIRE(url.fragment == "some-fragment");
        REQUIRE((std::string)url == urlString);
    }

    SECTION("From relative URL")
    {
        std::string urlString("https://fit.cvut.cz:1234/path/to/resource");
        std::string relativeUrl("./../not/2/anything");
        CUrl url = CUrl::FromRelativeUrl(CUrl(urlString), relativeUrl);
        REQUIRE((std::string)url == "https://fit.cvut.cz:1234/path/not/2/anything");
    }
}