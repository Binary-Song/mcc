#include "frontend.hpp"
#include "command_specs.hpp"

namespace mcc {
    void CommandSpecs::set_register_selection_bits(Command const& cmd, RegBitsOptions opt )
    {
        int src_reg_num = -1;

        auto get_reg_num = [](String const& reg_name) {
            if (reg_name == "r0" || reg_name == "R0") {
                return 0;
            } else if (reg_name == "r1" || reg_name == "R1") {
                return 1;
            } else if (reg_name == "r2" || reg_name == "R2") {
                return 2;
            } else if (reg_name == "r3" || reg_name == "R3") {
                return 3;
            } else {
                return -1;
            }
        };

        // dest reg
        if (!opt.ignore_dest) {
            if (cmd.args.size() >= opt.dest_arg_index + 1) {
                int reg_i = get_reg_num(cmd.args[opt.dest_arg_index]);
                if (reg_i != -1) {
                    bits[2] = reg_i % 2;
                    bits[3] = reg_i / 2;
                } else if (opt.require_dest) {
                    throw TranslationError("Rd required.");
                }
            } else if (opt.require_dest) {
                throw TranslationError("Rd required.");
            }
        }

        // src reg
        if (!opt.ignore_src) {
            if (cmd.args.size() >= 2) {
                int reg_i = get_reg_num(cmd.args[1]);
                if (reg_i != -1) {
                    bits[0] = reg_i % 2;
                    bits[1] = reg_i / 2;
                } else if (opt.require_dest) {
                    throw TranslationError("Rs required.");
                }
            } else if (opt.require_src) {
                throw TranslationError("Rs required.");
            }
        }
    }
}
