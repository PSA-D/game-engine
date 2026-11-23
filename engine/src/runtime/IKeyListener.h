#pragma once
#include "src/utils/InputCodes.h"

namespace engine
{
class IKeyListener
{
public:
    virtual ~IKeyListener() = default;

    virtual void onKeyDown(KeyCode key) = 0;
    virtual void onKeyUp(KeyCode key) = 0;
};
} // namespace engine
