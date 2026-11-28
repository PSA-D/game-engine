#pragma once

namespace engine::subsystems
{
    // Interne commands, dus niet voor de game developer
    class ICommand
    {
    public:
        virtual ~ICommand() = default;
        virtual void execute() = 0;
    };
} // namespace engine::subsystems
