//
// Created by Jordy on 16/11/2026.
//

#include "platform/IAudioBackend.h"
#include "platform/IAudioClip.h"
#include "platform/IInputSource.h"
#include "platform/INativeEventListener.h"
#include "platform/IPlatformFactory.h"
#include "platform/IPlatformPaths.h"
#include "platform/IRenderer.h"
#include "platform/ITexture.h"
#include "platform/IWindow.h"
#include "platform/VoiceHandle.h"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <type_traits>
#include <vector>

namespace engine::platform
{
namespace
{
// Zonder virtuele destructor lekt een delete via de basisklasse.
static_assert(std::has_virtual_destructor_v<IWindow>);
static_assert(std::has_virtual_destructor_v<IRenderer>);
static_assert(std::has_virtual_destructor_v<ITexture>);
static_assert(std::has_virtual_destructor_v<IInputSource>);
static_assert(std::has_virtual_destructor_v<INativeEventListener>);
static_assert(std::has_virtual_destructor_v<IAudioBackend>);
static_assert(std::has_virtual_destructor_v<IAudioClip>);
static_assert(std::has_virtual_destructor_v<IPlatformPaths>);
static_assert(std::has_virtual_destructor_v<IPlatformFactory>);

class TestWindow final : public IWindow
{
public:
    explicit TestWindow(const utils::Size& initialSize) : size(initialSize)
    {
    }

    [[nodiscard]] utils::Size getSize() const override
    {
        return size;
    }
    [[nodiscard]] utils::Size getPixelSize() const override
    {
        return size;
    }
    void setTitle(const std::string&) override
    {
    }
    void setFullscreen(bool) override
    {
    }
    void setSize(utils::Size newSize) override
    {
        size = newSize;
    }

private:
    utils::Size size;
};

class TestTexture final : public ITexture
{
public:
    TestTexture(int initialWidth, int initialHeight) : width(initialWidth), height(initialHeight)
    {
    }

    [[nodiscard]] int getWidth() const override
    {
        return width;
    }
    [[nodiscard]] int getHeight() const override
    {
        return height;
    }

private:
    int width;
    int height;
};

class TestRenderer final : public IRenderer
{
public:
    explicit TestRenderer(const utils::Size& initialOutputSize) : outputSize(initialOutputSize)
    {
    }

    void clear(const utils::Color&) override
    {
        ++calls;
    }
    void drawTexture(const ITexture&, const DrawParams&) override
    {
        ++calls;
    }
    void drawRect(const utils::Rect&, const utils::Color&, bool) override
    {
        ++calls;
    }
    void present() override
    {
        ++calls;
    }
    [[nodiscard]] std::unique_ptr<ITexture> createTexture(const ImageData& data) override
    {
        return std::make_unique<TestTexture>(data.size.width, data.size.height);
    }
    [[nodiscard]] utils::Size getOutputSize() const override
    {
        return outputSize;
    }
    [[nodiscard]] NativeHandles getNativeHandles() const override
    {
        return {};
    }

    [[nodiscard]] int getCalls() const
    {
        return calls;
    }

private:
    utils::Size outputSize;
    int calls{0};
};

class TestNativeEventListener final : public INativeEventListener
{
public:
    explicit TestNativeEventListener(bool claimsEvents) : claims(claimsEvents)
    {
    }

    [[nodiscard]] bool onNativeEvent(const void*) override
    {
        return claims;
    }

private:
    bool claims;
};

class TestInputSource final : public IInputSource
{
public:
    [[nodiscard]] std::optional<PlatformEvent> pollEvent() override
    {
        if (events.empty())
            return std::nullopt;

        PlatformEvent event = events.front();
        events.pop();
        return event;
    }
    void addNativeEventListener(INativeEventListener& listener) override
    {
        listeners.push_back(&listener);
    }
    void removeNativeEventListener(INativeEventListener& listener) override
    {
        std::erase(listeners, &listener);
    }

    void inject(const PlatformEvent& event)
    {
        events.push(event);
    }
    [[nodiscard]] std::size_t getListenerCount() const
    {
        return listeners.size();
    }

private:
    std::queue<PlatformEvent> events;
    std::vector<INativeEventListener*> listeners; // niet-bezittend
};

class TestAudioClip final : public IAudioClip
{
public:
    explicit TestAudioClip(float initialDuration) : duration(initialDuration)
    {
    }

    [[nodiscard]] float getDuration() const override
    {
        return duration;
    }

private:
    float duration;
};

class TestAudioBackend final : public IAudioBackend
{
public:
    [[nodiscard]] std::unique_ptr<IAudioClip> createClip(const AudioData& data) override
    {
        const int samplesPerSecond = data.channels * data.sampleRate;
        const float duration =
            samplesPerSecond > 0
                ? static_cast<float>(data.samples.size()) / static_cast<float>(samplesPerSecond)
                : 0.0F;
        return std::make_unique<TestAudioClip>(duration);
    }
    VoiceHandle play(const IAudioClip&, float, float, bool) override
    {
        return VoiceHandle{++lastId, 1};
    }
    void stop(VoiceHandle) override
    {
    }
    void setVoiceVolume(VoiceHandle, float) override
    {
    }
    void pause(VoiceHandle) override
    {
    }
    void resume(VoiceHandle) override
    {
    }
    void setMasterVolume(float) override
    {
    }

private:
    std::uint32_t lastId{0};
};

class TestPaths final : public IPlatformPaths
{
public:
    [[nodiscard]] std::filesystem::path getAssetRoot() const override
    {
        return "assets";
    }
    [[nodiscard]] std::filesystem::path getUserDataPath() const override
    {
        return "userdata";
    }
};

class TestFactory final : public IPlatformFactory
{
public:
    [[nodiscard]] std::unique_ptr<IWindow> createWindow(const WindowConfig& config) override
    {
        return std::make_unique<TestWindow>(config.size);
    }
    [[nodiscard]] std::unique_ptr<IRenderer> createRenderer(IWindow& window) override
    {
        return std::make_unique<TestRenderer>(window.getPixelSize());
    }
    [[nodiscard]] std::unique_ptr<IInputSource> createInputSource(IWindow&) override
    {
        return std::make_unique<TestInputSource>();
    }
    [[nodiscard]] std::unique_ptr<IAudioBackend> createAudioBackend() override
    {
        return std::make_unique<TestAudioBackend>();
    }
    [[nodiscard]] std::unique_ptr<IPlatformPaths> createPaths(const std::string&) override
    {
        return std::make_unique<TestPaths>();
    }
};

// Zet een vlag zodra hij vernietigd wordt, om de destructor via de basisklasse te testen.
class ProbeClip final : public IAudioClip
{
public:
    explicit ProbeClip(bool& destroyedFlag) : destroyed(destroyedFlag)
    {
    }
    ~ProbeClip() override
    {
        destroyed = true;
    }

    [[nodiscard]] float getDuration() const override
    {
        return 0.0F;
    }

private:
    bool& destroyed;
};
} // namespace

TEST_CASE("VoiceHandle is standaard ongeldig", "[platform]")
{
    const VoiceHandle empty{};
    CHECK_FALSE(empty.isValid());

    const VoiceHandle valid{.id = 3, .generation = 1};
    CHECK(valid.isValid());
    CHECK(valid == VoiceHandle{3, 1});
    CHECK_FALSE(valid == VoiceHandle{3, 2});
}

TEST_CASE("De factory maakt alle producten via de interfaces", "[platform]")
{
    TestFactory concreteFactory;
    IPlatformFactory& factory = concreteFactory;

    const std::unique_ptr<IWindow> window = factory.createWindow(WindowConfig{.size = {640, 480}});
    const std::unique_ptr<IRenderer> renderer = factory.createRenderer(*window);
    const std::unique_ptr<IInputSource> input = factory.createInputSource(*window);
    const std::unique_ptr<IAudioBackend> audio = factory.createAudioBackend();
    const std::unique_ptr<IPlatformPaths> paths = factory.createPaths("Test");

    CHECK(window != nullptr);
    CHECK(renderer != nullptr);
    CHECK(input != nullptr);
    CHECK(audio != nullptr);
    CHECK(paths != nullptr);
}

TEST_CASE("IWindow is implementeerbaar en werkt via het interface", "[platform]")
{
    TestFactory factory;
    const std::unique_ptr<IWindow> window = factory.createWindow(WindowConfig{.size = {640, 480}});

    CHECK(window->getSize().width == 640);
    CHECK(window->getPixelSize().height == 480);

    window->setTitle("Nieuwe titel");
    window->setFullscreen(true);
    window->setSize({800, 600});

    CHECK(window->getSize().width == 800);
    CHECK(window->getSize().height == 600);
}

TEST_CASE("IRenderer en ITexture zijn implementeerbaar", "[platform]")
{
    TestFactory factory;
    const std::unique_ptr<IWindow> window = factory.createWindow(WindowConfig{.size = {640, 480}});
    const std::unique_ptr<IRenderer> renderer = factory.createRenderer(*window);

    CHECK(renderer->getOutputSize().width == 640);
    CHECK(renderer->getNativeHandles().window == nullptr);
    CHECK(renderer->getNativeHandles().renderer == nullptr);

    ImageData image;
    image.size = {32, 16};
    const std::unique_ptr<ITexture> texture = renderer->createTexture(image);
    REQUIRE(texture != nullptr);
    CHECK(texture->getWidth() == 32);
    CHECK(texture->getHeight() == 16);

    renderer->clear({});
    renderer->drawTexture(*texture, DrawParams{});
    renderer->drawRect({}, {}, true);
    renderer->present();
}

TEST_CASE("IInputSource levert events in volgorde en daarna nullopt", "[platform]")
{
    TestInputSource concreteInput;
    IInputSource& input = concreteInput;

    CHECK_FALSE(input.pollEvent().has_value());

    concreteInput.inject(KeyEvent{});
    concreteInput.inject(QuitEvent{});

    const auto first = input.pollEvent();
    REQUIRE(first.has_value());
    CHECK(std::holds_alternative<KeyEvent>(*first));

    const auto second = input.pollEvent();
    REQUIRE(second.has_value());
    CHECK(std::holds_alternative<QuitEvent>(*second));

    CHECK_FALSE(input.pollEvent().has_value());
}

TEST_CASE("Native luisteraars kunnen aan- en afgemeld worden", "[platform]")
{
    TestInputSource concreteInput;
    IInputSource& input = concreteInput;
    TestNativeEventListener claimingListener{true};
    TestNativeEventListener ignoringListener{false};

    input.addNativeEventListener(claimingListener);
    input.addNativeEventListener(ignoringListener);
    CHECK(concreteInput.getListenerCount() == 2);

    input.removeNativeEventListener(claimingListener);
    CHECK(concreteInput.getListenerCount() == 1);

    INativeEventListener& listener = claimingListener;
    CHECK(listener.onNativeEvent(nullptr));
    CHECK_FALSE(ignoringListener.onNativeEvent(nullptr));
}

TEST_CASE("IAudioBackend en IAudioClip zijn implementeerbaar", "[platform]")
{
    TestFactory factory;
    const std::unique_ptr<IAudioBackend> audio = factory.createAudioBackend();

    AudioData data;
    data.sampleRate = 44100;
    data.channels = 2;
    data.samples.assign(88200, 0); // precies 1 seconde stereo

    const std::unique_ptr<IAudioClip> clip = audio->createClip(data);
    REQUIRE(clip != nullptr);
    CHECK(clip->getDuration() == 1.0F);

    const VoiceHandle first = audio->play(*clip, 1.0F, 1.0F, false);
    const VoiceHandle second = audio->play(*clip, 0.5F, 1.0F, true);
    CHECK(first.isValid());
    CHECK(second.isValid());
    CHECK_FALSE(first == second);

    audio->setVoiceVolume(first, 0.2F);
    audio->pause(first);
    audio->resume(first);
    audio->stop(first);
    audio->setMasterVolume(0.5F);
}

TEST_CASE("IPlatformPaths is implementeerbaar", "[platform]")
{
    TestFactory factory;
    const std::unique_ptr<IPlatformPaths> paths = factory.createPaths("Test");

    CHECK_FALSE(paths->getAssetRoot().empty());
    CHECK_FALSE(paths->getUserDataPath().empty());
}

TEST_CASE("Vernietigen via een basis-pointer roept de afgeleide destructor aan", "[platform]")
{
    bool destroyed = false;
    {
        const std::unique_ptr<IAudioClip> clip = std::make_unique<ProbeClip>(destroyed);
        CHECK_FALSE(destroyed);
    }
    CHECK(destroyed);
}
} // namespace engine::platform
