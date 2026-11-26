#pragma once

namespace engine::platform
{
    // Non-owning handles; validity depends on the lifetime of the native resources.
    struct NativeHandles
    {
        void* window{nullptr};
        void* renderer{nullptr};
    };
} // namespace engine::platform
