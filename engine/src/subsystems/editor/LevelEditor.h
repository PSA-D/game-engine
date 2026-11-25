#pragma once

namespace engine::subsystems
{
    // Leeg tot de editor-US. Signature volgens EditorV2-basis.
    // EngineImpl bezit hem; de GameLoop leent hem (LevelEditor&, zoals in het KD).
    class LevelEditor
    {
    public:
        void update() {}

        [[nodiscard]] bool isEditing() const
        {
            return false;
        }
    };
} // namespace engine::subsystems
