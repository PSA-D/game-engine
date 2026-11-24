#pragma once

namespace engine::subsystems
{
    class ICommand
    {
    public:
        virtual ~ICommand() = default;
        virtual void execute() = 0;
    };
} // namespace engine::subsystems
