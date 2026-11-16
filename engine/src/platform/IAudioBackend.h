//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_IAUDIOBACKEND_H
#define PROJECTSYSTEMARCHITECTURE_IAUDIOBACKEND_H

#pragma once

#include "platform/AudioData.h"
#include "platform/IAudioClip.h"
#include "platform/VoiceHandle.h"

#include <memory>

namespace engine::platform
{
// Speelt geluid af.
// Eigenaarschap en levensduur:
// - Wordt aangemaakt door IPlatformFactory::createAudioBackend(). De aanroeper bezit hem.
// - Moet vernietigd worden ná alle clips die hij heeft aangemaakt.
// - Clips mogen alleen met deze backend afgespeeld worden.
class IAudioBackend
{
public:
    IAudioBackend() = default;
    virtual ~IAudioBackend() = default;
    IAudioBackend(const IAudioBackend&) = delete;
    auto operator=(const IAudioBackend&) -> IAudioBackend& = delete;
    IAudioBackend(IAudioBackend&&) = delete;
    auto operator=(IAudioBackend&&) -> IAudioBackend& = delete;

    // De aanroeper bezit de clip. Hij moet vernietigd worden vóór deze backend.
    [[nodiscard]] virtual std::unique_ptr<IAudioClip> createClip(const AudioData& data) = 0;

    // Start een geluid. Geeft een ongeldige handle (id 0) terug als het niet lukt, bijvoorbeeld als er geen vrije stem meer is.
    virtual VoiceHandle play(const IAudioClip& clip, float volume, float pitch, bool loop) = 0;

    // Een ongeldige of verouderde handle wordt genegeerd.
    virtual void stop(VoiceHandle voice) = 0;
    virtual void setVoiceVolume(VoiceHandle voice, float volume) = 0;
    virtual void pause(VoiceHandle voice) = 0;
    virtual void resume(VoiceHandle voice) = 0;

    virtual void setMasterVolume(float volume) = 0;
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_IAUDIOBACKEND_H
