#pragma once

#include "input/InputSubsystem.h"
#include "utils/Vector2.h"

namespace engine
{
    class InputFacade
    {
    public:
        InputFacade() = default;
        ~InputFacade() = default;
        InputFacade(const InputFacade&) = delete;
        InputFacade& operator=(const InputFacade&) = delete;
        InputFacade(InputFacade&&) = delete;
        InputFacade& operator=(InputFacade&&) = delete;

        static bool getKey(KeyCode key);     // Checkt of de key wordt ingedrukt
        static bool getKeyDown(KeyCode key); // enz.
        static bool getKeyUp(KeyCode key);
        static bool getMouseButton(MouseButton button);
        static bool getMouseButtonDown(MouseButton button);
        static bool getMouseButtonUp(MouseButton button);

        static Vector2 getMousePosition();
        static Vector2 getMouseDelta();
        static Vector2 getMouseWheel();

        static void addKeyListener(IKeyListener* listener);
        static void removeKeyListener(IKeyListener* listener);
        static void addMouseListener(IMouseListener* listener);
        static void removeMouseListener(IMouseListener* listener);

    private:
        // friend class, zodat deze bij de private methodes kan.
        friend class EngineImpl;

        static void init(subsystems::InputSubsystem* subsystem);
        static void shutdown();
        // init() en shutdown() zijn nodig
        // om het static InputSubsystem te initializeren

        static subsystems::InputSubsystem* subsystem_;
    };
} // namespace engine
