//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_NATIVEHANDLES_H
#define PROJECTSYSTEMARCHITECTURE_NATIVEHANDLES_H

#pragma once

namespace engine::platform
{
// Non-owning handles voor debug-UI integratie.
// Deze zijn alleen geldig zolang het venster en de renderer waaruit ze afkomstig zijn bestaan.
// Verwijder ze niet en sla ze niet op.

struct NativeHandles
{
    void* window{nullptr};
    void* renderer{nullptr};
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_NATIVEHANDLES_H
