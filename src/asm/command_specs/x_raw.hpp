#pragma once 
#include "common/precompiled.hpp"
#include "asm/command_specs.hpp"
#include "asm/error.hpp"
#include "common/utility/functions/string_to_int.hpp"
namespace mcc {
    class CommandRaw : public CommandSpecs
    {
    public:
        int size() const override
        {
            return 1;
        }

        void to_binary(Command const& cmd) override;
    };
} // namespace mcc
