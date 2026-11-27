#pragma once

#include "core/ComponentRegistry.h"
#include "runtime/Component.h"
#include "runtime/DataNode.h"
#include "runtime/GameObject.h"
#include "runtime/Level.h"
#include "runtime/Region.h"

#include <cstdint>
#include <memory>
#include <sstream>
#include <string>

namespace engine::subsystems
{
    // Een geregistreerd test-component met twee velden.
    class TestSprite final : public Component
    {
    public:
        using Component::Component;

        std::string sprite;
        int layer{0};
    };

    // Dit component wordt bewust niet geregistreerd.
    class TestUnregistered final : public Component
    {
    public:
        using Component::Component;
    };

    // Pas deze functie aan op de echte signatuur van ComponentRegistry::registerType.
    inline void registerTestComponents(ComponentRegistry& registry)
    {
        using engine::DataNode;

        registry.registerType<TestSprite>(
            "TestSprite",
            [](const TestSprite& component)
            {
                auto node = DataNode::makeObject();
                node.set("sprite", DataNode(component.sprite));
                node.set("layer", DataNode(component.layer));
                return node;
            },
            [](GameObject& owner, const DataNode& node) -> TestSprite*
            {
                const auto* sprite = node.get("sprite");
                const auto* layer = node.get("layer");

                if (sprite == nullptr || layer == nullptr)
                    return nullptr;

                const auto spriteValue = sprite->tryString();
                const auto layerValue = layer->tryInt();

                if (!spriteValue || !layerValue)
                    return nullptr;

                auto& component = owner.addComponent<TestSprite>();

                component.sprite = *spriteValue;
                component.layer = static_cast<int>(*layerValue);

                return &component;
            });
    }

    // Een voorbeeldlevel: twee regions, een hiërarchie, transforms en componenten.
    //   Start: Player (met TestSprite, uitgeschakeld) > Weapon, Camera
    //   Boss:  Boss   (actieve region)
    inline auto buildSampleLevel() -> std::unique_ptr<Level>
    {
        auto level = std::make_unique<Level>("Level 1");

        auto& start = level->createRegion("Start");
        auto& player = start.createGameObject("Player");
        player.setTag("Player");
        player.getTransform().setLocalPosition({1.5F, -2.0F});
        player.getTransform().setLocalRotation(90.0F);
        player.getTransform().setLocalScale({2.0F, 3.0F});
        auto& sprite = player.addComponent<TestSprite>();
        sprite.sprite = "hero.png";
        sprite.layer = 2;
        sprite.setEnabled(false);

        auto& weapon = start.createGameObject("Weapon");
        static_cast<void>(weapon.setParent(&player, false));
        weapon.getTransform().setLocalPosition({.x = 0.5F, .y = 0.0F});

        start.createGameObject("Camera");

        auto& bossRegion = level->createRegion("Boss");
        bossRegion.createGameObject("Boss").setTag("Enemy");
        level->activateRegion(bossRegion);

        return level;
    }

    // Beschrijft een level met alleen de publieke runtime-API, onafhankelijk van de serializer.
    // Twee levels met dezelfde beschrijving zijn voor de game hetzelfde.
    inline auto snapshot(const Level& level) -> std::string
    {
        std::ostringstream out;
        out << "level " << level.getName() << "\n";
        const auto* active = level.getActiveRegion();
        for (const auto* region : level.getRegions())
        {
            out << "region " << region->getName() << (region == active ? " (actief)" : "") << "\n";
            for (const auto* gameObject : region->getGameObjects())
            {
                const auto position = gameObject->getTransform().getLocalPosition();
                const auto scale = gameObject->getTransform().getLocalScale();
                out << "  object " << gameObject->getName() << " tag=" << gameObject->getTag()
                    << " parent=" << (gameObject->getParent() != nullptr ? gameObject->getParent()->getName() : "-")
                    << " pos=" << position.x << "," << position.y
                    << " rot=" << gameObject->getTransform().getLocalRotation() << " scale=" << scale.x << ","
                    << scale.y << "\n";
                for (const auto* sprite : gameObject->getComponents<TestSprite>())
                {
                    out << "    sprite " << sprite->sprite << " layer=" << sprite->layer
                        << " enabled=" << sprite->isEnabled() << "\n";
                }
            }
        }
        return out.str();
    }
} // namespace engine::subsystems
