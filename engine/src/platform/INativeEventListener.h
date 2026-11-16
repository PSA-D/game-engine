//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_INATIVEEVENTLISTENER_H
#define PROJECTSYSTEMARCHITECTURE_INATIVEEVENTLISTENER_H

#pragma once

namespace engine::platform
{
// Callback voor ruwe, onvertaalde events (bijvoorbeeld SDL_Event).
// Alleen bedoeld voor debug-UI-integraties (ImGui, RmlUi).
// De pointer is ruw zodat dit interface geen SDL kent.
// De implementatie weet zelf welk type erachter zit.
// Contract:
// - Wordt op de hoofdthread aangeroepen, vanuit IInputSource::pollEvent(), vóór het event vertaald wordt.
// - Geeft true terug als de UI het event claimt. Muis- en toetsenbordevents worden dan niet aan de game doorgegeven.
//   WindowEvent en QuitEvent worden nooit weggefilterd.
class INativeEventListener
{
public:
    INativeEventListener() = default;
    virtual ~INativeEventListener() = default;
    INativeEventListener(const INativeEventListener&) = delete;
    auto operator=(const INativeEventListener&) -> INativeEventListener& = delete;
    INativeEventListener(INativeEventListener&&) = delete;
    auto operator=(INativeEventListener&&) -> INativeEventListener& = delete;

    [[nodiscard]] virtual bool onNativeEvent(const void* nativeEvent) = 0;
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_INATIVEEVENTLISTENER_H
