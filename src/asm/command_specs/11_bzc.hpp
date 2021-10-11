#pragma once 
#include "common/precompiled.hpp"
#include "asm/command_specs.hpp"
namespace mcc {
    class CommandBranchZeroCarry : public CommandSpecs
    {
    public:
        int size() const override
        {
            return 1;
        }

        void to_binary(Command const& cmd) override
        {
            this->set_bits(0xB0);
            set_register_selection_bits(cmd);
        }
    };
} // namespace mcc
