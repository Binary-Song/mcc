#pragma once 
#include "common/precompiled.hpp"
#include "asm/command_specs.hpp"
namespace mcc {
    class CommandJump : public CommandSpecs
    {
    public:
        bool use_ram = false;
        CommandJump(bool use_ram = false) 
        {
            this->use_ram = use_ram;
        }
        int size() const override
        {
            return 2;
        }

        void to_binary(Command const& cmd);
    };
} // namespace mcc
