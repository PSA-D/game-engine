//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_IINPUTSOURCE_H
#define PROJECTSYSTEMARCHITECTURE_IINPUTSOURCE_H

#pragma once

#include "platform/INativeEventListener.h"
#include "platform/PlatformEvent.h"

#include <optional>

namespace engine::platform
{
// Levert de events van het platform (toetsen, muis, window, afsluiten).
// Eigenaarschap en levensduur:
// - Wordt aangemaakt door IPlatformFactory::createInputSource(). De aanroeper bezit hem.
// - Moet vernietigd worden vóór het window waarmee hij gemaakt is.
class IInputSource
{
public:
    IInputSource() = default;
    virtual ~IInputSource() = default;
    IInputSource(const IInputSource&) = delete;
    auto operator=(const IInputSource&) -> IInputSource& = delete;
    IInputSource(IInputSource&&) = delete;
    auto operator=(IInputSource&&) -> IInputSource& = delete;

    // Geeft het volgende event, of std::nullopt als er geen meer zijn. Aanroepen in een lus.
    [[nodiscard]] virtual std::optional<PlatformEvent> pollEvent() = 0;

    // Alleen voor debug-UI-integraties. Luisteraars zien het ruwe event voordat het vertaald wordt.
    // - De input source bezit of verwijdert een luisteraar nooit.
    // - Een luisteraar moet zich afmelden vóór hij vernietigd wordt.
    virtual void addNativeEventListener(INativeEventListener& listener) = 0;
    virtual void removeNativeEventListener(INativeEventListener& listener) = 0;
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_IINPUTSOURCE_H
