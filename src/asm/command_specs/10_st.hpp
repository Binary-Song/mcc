#pragma once 
#include "common/precompiled.hpp"
#include "asm/command_specs.hpp"
namespace mcc {
    class CommandStore : public CommandSpecs
    {
    public:
        int size() const override
        {
            return 2;
        }

        void to_binary(Command const& cmd) override;
    };
} // namespace mcc
