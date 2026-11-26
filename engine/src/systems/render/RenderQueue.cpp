#include "systems/render/RenderQueue.h"

#include <algorithm>
#include <functional>

namespace engine
{
    void RenderQueue::add(RenderItem item)
    {
        items.push_back(item);
    }

    void RenderQueue::sort()
    {
        const std::less<const platform::ITexture*> textureLess;
        std::stable_sort(items.begin(),
                         items.end(),
                         [&textureLess](const RenderItem& left, const RenderItem& right)
                         {
                             if (left.sortingLayer != right.sortingLayer)
                             {
                                 return left.sortingLayer < right.sortingLayer;
                             }
                             if (left.orderInLayer != right.orderInLayer)
                             {
                                 return left.orderInLayer < right.orderInLayer;
                             }
                             return textureLess(left.texture, right.texture);
                         });
    }

    const std::vector<RenderItem>& RenderQueue::getItems() const noexcept
    {
        return items;
    }

    void RenderQueue::clear() noexcept
    {
        items.clear();
    }
} // namespace engine
