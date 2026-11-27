#pragma once

#include "platform/IWindow.h"
#include "platform/WindowConfig.h"

namespace engine::platform
{
    // Een venster zonder scherm: onthoudt alleen zijn instellingen.
    // Voor tests en CI, waar geen echt venster geopend kan worden.
    class HeadlessWindow final : public IWindow
    {
    public:
        explicit HeadlessWindow(WindowConfig config);

        [[nodiscard]] std::string getTitle() const override;
        [[nodiscard]] Size getSize() const override;
        [[nodiscard]] Size getPixelSize() const override;

        void setTitle(const std::string& title) override;
        void setFullscreen(bool enabled) override;
        void setSize(Size size) override;

        [[nodiscard]] const WindowConfig& getConfig() const;

    private:
        WindowConfig config;
    };
} // namespace engine::platform
