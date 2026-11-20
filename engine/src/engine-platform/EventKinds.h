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
        A, B, C, D, E,
        F, G, H, I, J,
        K, L, M, N, O,
        P, Q, R, S, T,
        U, V, W, X, Y, Z,
        Num01, Num02,
        Num03, Num04,
        Num05, Num06,
        Num07, Num08,
        Num09, Num00,
        Space,
        Enter,
        Escape,
        Tab,
        Backspace,
        ArrowUp, ArrowDown,
        ArrowLeft, ArrowRight,
        PageUp, PageDown,
        Home, End,
        F1, F2, F3, F4,
        F5, F6, F7, F8,
        F9, F10, F11, F12,
    };
}