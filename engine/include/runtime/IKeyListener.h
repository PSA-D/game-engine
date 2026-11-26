#pragma once

#include "utils/KeyCode.h"

namespace engine
{
    // Event listener voor de toetsen (volgens het KD).
    // onKeyDown komt één keer per indrukken (geen herhaling bij vasthouden).
    // Voor Shift/Ctrl/Alt vraag je de toestand op via de input (getKey).
    class IKeyListener
    {
    public:
        virtual ~IKeyListener() = default;
        virtual void onKeyDown(KeyCode key) = 0;
        virtual void onKeyUp(KeyCode key) = 0;
    };
} // namespace engine
