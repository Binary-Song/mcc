#include "11_bzc.hpp"
#include "asm/frontend.hpp"
namespace mcc {
    void CommandBranchZeroCarry::to_binary(Command const& cmd) 
    {
        if (cmd.args.size() != 1) {
            throw TranslationError("bzc requires 1 argument");
        }

        this->set_bits(0xB0);
        this->set_register_selection_bits(cmd);
 
        bool is_indirect_mode = cmd.command.back() == '&';
        this->set_addressing_mode_bits(!is_indirect_mode);

        unsigned int dest = string_to_int(cmd.args[1]);
        this->set_data_bits(dest);

        this->has_data = true;
    }
}
