#include "facades/Time.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

namespace engine
{
    // Time::advance() is private (alleen de GameLoop mag de tijd vooruitzetten).
    // Deze struct is in Time.h als friend opgegeven, zodat de tests het toch kunnen.
    struct TimeTestAccess
    {
        static void advance(float frameDeltaTime)
        {
            Time::advance(frameDeltaTime);
        }
    };

    TEST_CASE("Time geeft bij tijdschaal 1 dezelfde geschaalde en ongeschaalde deltaTime", "[unit][core]")
    {
        Time::setTimeScale(1.0F);

        TimeTestAccess::advance(0.016F);

        CHECK(Time::getDeltaTime() == Catch::Approx(0.016F));
        CHECK(Time::getUnscaledDeltaTime() == Catch::Approx(0.016F));
    }

    TEST_CASE("Time schaalt deltaTime met de tijdschaal, maar unscaledDeltaTime niet", "[unit][core]")
    {
        Time::setTimeScale(0.5F);

        TimeTestAccess::advance(0.02F);

        CHECK(Time::getTimeScale() == Catch::Approx(0.5F));
        CHECK(Time::getDeltaTime() == Catch::Approx(0.01F));
        CHECK(Time::getUnscaledDeltaTime() == Catch::Approx(0.02F));

        Time::setTimeScale(1.0F);
    }

    TEST_CASE("Time met tijdschaal 0 (pauze) geeft deltaTime 0", "[unit][core]")
    {
        Time::setTimeScale(0.0F);

        TimeTestAccess::advance(0.016F);

        CHECK(Time::getDeltaTime() == 0.0F);
        CHECK(Time::getUnscaledDeltaTime() == Catch::Approx(0.016F));

        Time::setTimeScale(1.0F);
    }

    TEST_CASE("Time maakt van een negatieve tijdschaal 0", "[unit][core]")
    {
        Time::setTimeScale(-2.0F);

        CHECK(Time::getTimeScale() == 0.0F);

        Time::setTimeScale(1.0F);
    }
} // namespace engine
