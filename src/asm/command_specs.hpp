#pragma once 
#include "common/precompiled.hpp"
#include "asm/error.hpp"
#include <array>
namespace mcc {
    
    class Command;
    class CommandSpecs
    {
    public:

        SharedPtr<Map<String, int>> addr_map;
        std::array<bool, 8> bits;
        std::array<bool, 8> data_bits;
        bool has_data = false;
 
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

        unsigned char data() const
        {
            unsigned char byte = 0;
            int offset = 0;
            for (auto&& bit : data_bits) {
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


        void set_bits(unsigned char c)
        {
            for (auto&& bit : bits) {
                bit = c % 2;
                c /= 2;
            }
        }
        
        void set_data_bits(unsigned char c)
        {
            for (auto&& bit: data_bits) {
                bit = c % 2;
                c /= 2;
            }
        }

        void and_mask_bits(unsigned char mask)
        {
            char bits = mask & byte();
            set_bits(bits);
        }

        void or_mask_bits(unsigned char mask)
        {
            char bits = mask | byte();
            set_bits(bits);
        }

        void set_addressing_mode_bits(bool direct_mode)
        {
            if (!direct_mode) {
                // indirect mode 
                this->and_mask_bits(0xFE); // set 0
            }else{
                // direct mode 
                this->or_mask_bits(0x01); // set 1
            }
        }

        void set_storage_device_bit(bool use_ram)
        {
            if (use_ram) { 
                this->and_mask_bits(0xFD); // set 0
            } else{ 
                this->or_mask_bits(0x02);// set 1
            }
        }

        void set_register_selection_bits(Command const& cmd, RegBitsOptions opt = RegBitsOptions());
    };
} // namespace mcc
