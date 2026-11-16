//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_IAUDIOCLIP_H
#define PROJECTSYSTEMARCHITECTURE_IAUDIOCLIP_H

#pragma once

namespace engine::platform
{
// Een geladen geluid dat afgespeeld kan worden.
// Eigenaarschap en levensduur:
// - Wordt aangemaakt door IAudioBackend::createClip(). De aanroeper bezit hem (unique_ptr).
// - Mag alleen afgespeeld worden door de backend die hem heeft aangemaakt.
// - Moet vernietigd worden vóór die backend.
class IAudioClip
{
public:
    IAudioClip() = default;
    virtual ~IAudioClip() = default;
    IAudioClip(const IAudioClip&) = delete;
    auto operator=(const IAudioClip&) -> IAudioClip& = delete;
    IAudioClip(IAudioClip&&) = delete;
    auto operator=(IAudioClip&&) -> IAudioClip& = delete;

    [[nodiscard]] virtual float getDuration() const = 0; // in seconden
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_IAUDIOCLIP_H
