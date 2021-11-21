#include "10_st.hpp"
#include "asm/frontend.hpp"
namespace mcc {
    void CommandStore::to_binary(Command const& cmd) 
    { 
        if (cmd.args.size() != 2) {
            throw TranslationError("st requires 2 argument");
        }

        this->set_bits(0xA0);
        this->set_register_selection_bits(cmd);
 
        bool is_indirect_mode = cmd.command.back() == '&';
        this->set_addressing_mode_bits(!is_indirect_mode);

        unsigned int dest = string_to_int(cmd.args[1]);
        this->set_data_bits(dest);

        this->has_data = true;
    }
}