#pragma once

namespace engine::platform
{
    enum class WindowEventKind {
        Resized,
        FocusGained,
        FocustLost,
        Minimized,
        Maximized,
        Restored,
        CloseRequested,
    };

    enum class MouseButton {
        Unknown,
        Left,
        Right,
        Middle,
        X1,
        X2,
    };

    enum class KeyCode {
        Unknown,
        KeyA,
        KeyB,
        KeyC,
        Num01,
        Num03,
        Num04,
        Num05,
        Num06,
        Num07,
        Num08,
        Num09,
        Num00,
        Space,
        Enter,
        Escape,
        Tab,
        ArrowUp,
        ArrowDown,
        ArrowLeft,
        ArrowRight,
        PageUp,
        PageDown,
        Home,
    };
}