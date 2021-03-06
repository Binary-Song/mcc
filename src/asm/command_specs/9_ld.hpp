#pragma once 
#include "common/precompiled.hpp"
#include "asm/command_specs.hpp"
namespace mcc {
    class CommandLoad : public CommandSpecs
    {
    public:
        bool use_ram = false;
        CommandLoad(bool use_ram = false) 
        {
            this->use_ram = use_ram;
        }
        int size() const override
        {
            return 2;
        } 
        void to_binary(Command const& cmd) override;
    };
} // namespace mcc
