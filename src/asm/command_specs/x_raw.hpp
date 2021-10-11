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

        void to_binary(Command const& cmd) override
        {
            try
            {
                this->set_bits(char(string_to_int(cmd.args[0]))); 
                set_register_selection_bits(cmd);
            }
            catch(ConversionError const& e)
            {
                throw TranslationError("Raw data not valid");
            }
        }
    };
} // namespace mcc
