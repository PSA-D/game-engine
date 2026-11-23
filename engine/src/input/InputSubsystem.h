#pragma once

#include "src/runtime/IKeyListener.h"
#include "src/runtime/IMouseListener.h"

#include <vector>

namespace engine::systems
{
// temp class
class InputSubsystem
{
public:
    void addKeyListener(IKeyListener* listener);
    void removeKeyListener(IKeyListener* listener);
    void addMouseListener(IMouseListener* listener);
    void removeMouseListener(IMouseListener* listener);

private:
    std::vector<IKeyListener*> keyListeners_;
    std::vector<IMouseListener*> mouseListeners_;
};
} // namespace engine::systems
