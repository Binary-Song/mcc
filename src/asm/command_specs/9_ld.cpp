#include "9_ld.hpp"
#include "asm/frontend.hpp"
namespace mcc {
    void CommandLoad::to_binary(Command const& cmd)    
    {
        if (cmd.args.size() != 2) {
            throw TranslationError("ld requires 2 arguments");
        }

        this->set_bits(0x90);
        this->set_register_selection_bits(cmd);
 
        bool is_indirect_mode = cmd.command.back() == '&';
        this->set_addressing_mode_bits(!is_indirect_mode);

        unsigned int dest = string_to_int(cmd.args[1]);
        this->set_data_bits(dest);

        this->has_data = true;
    } 
}
