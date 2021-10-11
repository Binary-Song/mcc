#pragma once 
#include "common/precompiled.hpp"
#include "asm/error.hpp"
#include <array>
namespace mcc {



    class CommandSpecs
    {
    public:

        SharedPtr<Map<String, int>> addr_map;
        std::array<bool, 8> bits;


        virtual int size() const = 0;
        virtual void to_binary(Command const& cmd) = 0;
        virtual ~CommandSpecs() {}

        unsigned char byte() const
        {
            unsigned char byte = 0;
            int offset = 0;
            for (auto&& bit : bits) {
                byte += (bit << offset);
                offset++;
            }
            return byte;
        }

    protected:

        struct RegBitsOptions
        {
            bool ignore_dest = false;
            bool ignore_src = false;
            int dest_arg_index = 0;
            int src_arg_index = 1;
            bool require_dest = false;
            bool require_src = false;
        };


        void set_bits(char c)
        {
            for (auto&& bit : bits) {
                bit = c % 2;
                c /= 2;
            }
        }

        void set_register_selection_bits(Command const& cmd,RegBitsOptions opt = RegBitsOptions())
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
                    } else if(opt.require_dest){
                        throw TranslationError("Rd required.");
                    }
                } else if(opt.require_dest) {
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
                    } else if(opt.require_dest){
                        throw TranslationError("Rs required.");
                    }
                } else if(opt.require_src) {
                    throw TranslationError("Rs required.");
                }
            }
        }
    };
} // namespace mcc
