#pragma once

#include "ICommand.h"

#include <functional>
#include <utility>

namespace engine::subsystems
{
    class FunctionCommand : public ICommand
    {
    public:
        FunctionCommand(std::function<void()> function)
            : function_(std::move(function))
        {
        }
        void execute() override
        {
            function_();
        }

    private:
        std::function<void()> function_;
    };
} // namespace engine::subsystems
