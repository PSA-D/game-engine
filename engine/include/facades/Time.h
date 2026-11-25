#pragma once

namespace engine
{
    class GameLoop;

    // Facade voor de tijd van het huidige frame, zoals Time in Unity.
    // De GameLoop vult de waarden aan het begin van elke tick; de game kan ze alleen lezen
    // en de tijdschaal aanpassen (bijvoorbeeld 0 voor pauze, 0.5 voor slow motion).
    class Time
    {
    public:
        Time() = delete;

        // Seconden sinds het vorige frame, vermenigvuldigd met de tijdschaal.
        [[nodiscard]] static float getDeltaTime() noexcept
        {
            return deltaTime;
        }

        // Seconden sinds het vorige frame, zonder tijdschaal (voor UI, debug en FPS-meting).
        [[nodiscard]] static float getUnscaledDeltaTime() noexcept
        {
            return unscaledDeltaTime;
        }

        [[nodiscard]] static float getTimeScale() noexcept
        {
            return timeScale;
        }

        static void setTimeScale(float scale) noexcept
        {
            timeScale = scale < 0.0F ? 0.0F : scale;
        }

    private:
        friend class GameLoop;
        friend struct TimeTestAccess;

        static void advance(float frameDeltaTime) noexcept
        {
            unscaledDeltaTime = frameDeltaTime;
            deltaTime = frameDeltaTime * timeScale;
        }

        static inline float deltaTime{0.0F};
        static inline float unscaledDeltaTime{0.0F};
        static inline float timeScale{1.0F};
    };
} // namespace engine
