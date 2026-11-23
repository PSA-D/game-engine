#pragma once

#include "platform/Events.h"

namespace engine
{
// Event listener voor de toetsen
class IKeyListener
{
public:
    virtual ~IKeyListener() = default;
    virtual void onKeyDown(const platform::KeyEvent& event) = 0;
    virtual void onKeyUp(const platform::KeyEvent& event) = 0;
};
} // namespace engine
