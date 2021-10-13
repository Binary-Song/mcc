#pragma once

#include "common/precompiled.hpp"

#include "asm/frontend.hpp"
#include "asm/command_specs/command_specs.hpp"
#include "asm/error.hpp"
  
#include <cstdio>
#include <fmt/os.h>
#include "specs.hpp"


namespace mcc {

    class Backend
    {
    public:

        class Error
        {
        public:
            String _what;
            Error(StringView what = "Unknown error")
            {
                this->_what = what;
            }
            String what() const
            {
                return _what;
            }
        };
 
        OutputFile output;

        Backend(Path const& output)
        {
            this->output.open(output.string());
        }

        void generate(Vector<Command> const& commands)
        {
           // auto&& addr_map = std::make_shared<Map<String, int>>(compute_label_addr(commands));
            for (auto&& command : commands) {
                SharedPtr<CommandSpecs> sp = specs[command.command];
                //sp->addr_map = addr_map;
                sp->to_binary(command);
                unsigned char bt = sp->byte(); 
                output.write(bt); 
                if(sp->has_data) {
                    auto databyte = sp->data();
                    output.write(databyte); 
                }
            }
        }

    private:
 
    };
}