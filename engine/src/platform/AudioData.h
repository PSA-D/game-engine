//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_AUDIODATA_H
#define PROJECTSYSTEMARCHITECTURE_AUDIODATA_H

#pragma once

#include <cstdint>
#include <vector>

namespace engine::platform
{
// Gedecodeerd geluid (voorlopige opzet, kan veranderen met de decoderkeuze).
struct AudioData
{
    int sampleRate{0};                  // in Hz
    int channels{0};                    // 1 = mono, 2 = stereo
    std::vector<std::int16_t> samples;  // PCM 16-bit, kanalen afwisselend (interleaved)
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_AUDIODATA_H
