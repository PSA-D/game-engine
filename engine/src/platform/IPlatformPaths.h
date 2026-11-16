//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_IPLATFORMPATHS_H
#define PROJECTSYSTEMARCHITECTURE_IPLATFORMPATHS_H

#pragma once

#include <filesystem>

namespace engine::platform
{
// Vaste mappen van het platform.
// Eigenaarschap en levensduur:
// - Wordt aangemaakt door IPlatformFactory::createPaths(). De aanroeper bezit hem.
// - Heeft geen afhankelijkheid van een window en kan dus als eerste bestaan.
class IPlatformPaths
{
public:
    IPlatformPaths() = default;
    virtual ~IPlatformPaths() = default;
    IPlatformPaths(const IPlatformPaths&) = delete;
    auto operator=(const IPlatformPaths&) -> IPlatformPaths& = delete;
    IPlatformPaths(IPlatformPaths&&) = delete;
    auto operator=(IPlatformPaths&&) -> IPlatformPaths& = delete;

    [[nodiscard]] virtual std::filesystem::path getAssetRoot() const = 0;
    [[nodiscard]] virtual std::filesystem::path getUserDataPath() const = 0;
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_IPLATFORMPATHS_H
