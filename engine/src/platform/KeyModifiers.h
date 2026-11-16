//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_KEYMODIFIERS_H
#define PROJECTSYSTEMARCHITECTURE_KEYMODIFIERS_H

#pragma once

namespace engine::platform
{
// Welke modificatietoetsen werden ingedrukt toen een key event plaatsvond
struct KeyModifiers
{
    bool shift{false};
    bool ctrl{false};
    bool alt{false};
    bool gui{false}; // Windows key / Cmd key
    bool capsLock{false};
    bool numLock{false};
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_KEYMODIFIERS_H
