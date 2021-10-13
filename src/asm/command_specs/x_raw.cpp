#include "x_raw.hpp"
#include "asm/error.hpp"
#include "common/utility/functions/string_to_int.hpp"
#include "asm/frontend.hpp"
#include "common/precompiled.hpp"

namespace mcc {
    void CommandRaw::to_binary(Command const& cmd)
    {
        try
        {
            this->set_bits(unsigned char(string_to_int(cmd.args[0]))); 
        }
        catch(ConversionError const& e)
        {
            throw TranslationError("Raw data not valid");
        }
    }
}
