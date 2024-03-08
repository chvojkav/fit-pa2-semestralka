#include "catch.hpp"
#include "../src/CUrlAuthority.h"

TEST_CASE("CUrlAuthority")
{
    SECTION("Basic parsing")
    {
        CUrlAuthority auth("//chvojvoj@fit.cvut.cz:987");
        REQUIRE(auth.userinfo == "chvojvoj");
        REQUIRE(auth.host == "fit.cvut.cz");
        REQUIRE(auth.port == "987");
        REQUIRE((std::string)auth == "//chvojvoj@fit.cvut.cz:987");
    }

    SECTION("Has port")
    {
        CUrlAuthority auth;
        REQUIRE(!auth.hasPort());
        auth = CUrlAuthority("//chvojvoj@fit.cvut.cz:987");
        REQUIRE(auth.hasPort());
    }

    SECTION("Has userinfo")
    {
        CUrlAuthority auth;
        REQUIRE(!auth.hasUserInfo());
        auth = CUrlAuthority("//chvojvoj@fit.cvut.cz:987");
        REQUIRE(auth.hasUserInfo());
    }
}

