#pragma once

#include "src/runtime/IKeyListener.h"
#include "src/utils/InputCodes.h"

#include <iostream>
#include <ostream>

class PlayerInput : public engine::IKeyListener
{
public:
    void onKeyDown(const engine::platform::KeyEvent& event) override
    {
        if (event.key == engine::KeyCode::ArrowLeft)
        {
            std::cout << "ArrowLeft Key down" << '\n';
        }
    }
    void onKeyUp(const engine::platform::KeyEvent& event) override
    {
        if (event.key == engine::KeyCode::ArrowLeft)
        {
            std::cout << "ArrowLeft Key up" << '\n';
        }
    }
};
