#pragma once

#include "systems/render/RenderItem.h"

#include <vector>

namespace engine
{
    class RenderQueue
    {
    public:
        void add(RenderItem item);
        void sort();
        [[nodiscard]] const std::vector<RenderItem>& getItems() const noexcept;
        void clear() noexcept;

    private:
        std::vector<RenderItem> items;
    };
} // namespace engine
