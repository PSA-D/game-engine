//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_VOICEHANDLE_H
#define PROJECTSYSTEMARCHITECTURE_VOICEHANDLE_H

#pragma once

#include <cstdint>

namespace engine::platform
{
// Verwijzing naar één afspelend geluid. Wordt teruggegeven door IAudioBackend::play().
// id 0 betekent ongeldig. De generation voorkomt hergebruik van verouderde handles
struct VoiceHandle
{
    std::uint32_t id{0};
    std::uint32_t generation{0};

    [[nodiscard]] bool isValid() const
    {
        return id != 0;
    }

    bool operator==(const VoiceHandle&) const = default;
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_VOICEHANDLE_H
