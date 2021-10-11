#pragma once

#include "common/precompiled.hpp"

#include "asm/frontend.hpp"
#include "asm/command_specs/command_specs.hpp"
#include "asm/error.hpp"
 

#include <cstdio>
#include <fmt/os.h>

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

#define COMMAND_SHARED_PTR(cmd) std::make_shared<cmd>(cmd())
        Map<String, SharedPtr<CommandSpecs>> specs = {
            {"raw", COMMAND_SHARED_PTR(CommandRaw) },
            {"clr", COMMAND_SHARED_PTR(CommandClear) },
            {"add", COMMAND_SHARED_PTR(CommandAdd)},
            {"sub", COMMAND_SHARED_PTR(CommandSubtract)},
            {"mov", COMMAND_SHARED_PTR(CommandMove)},

            {"inc", COMMAND_SHARED_PTR(CommandIncrease)},
            {"dec", COMMAND_SHARED_PTR(CommandDecrease)},
            {"ld",  COMMAND_SHARED_PTR(CommandLoad)},
            {"st",  COMMAND_SHARED_PTR(CommandStore)},
            {"bzc", COMMAND_SHARED_PTR(CommandBranchZeroCarry)}, 
        };
#undef COMMAND_SHARED_PTR

        OutputFile output;

        Backend(Path const& output)
        {
            this->output.open(output.string());
        }

        void generate(Vector<Command> const& commands)
        {
            auto&& addr_map = std::make_shared<Map<String, int>>(compute_label_addr(commands));
            for (auto&& command : commands) {
                SharedPtr<CommandSpecs> sp = specs[command.command];
                sp->addr_map = addr_map;
                sp->to_binary(command);
                auto bt = sp->byte();
                output.write(bt);
            }
        }

    private:

        Map<String, int> compute_label_addr(Vector<Command> const& commands)
        {
            Map<String, int> label_addr;
            int curr_addr = 0;
            for (auto&& cmd : commands) {
                if (cmd.label != "") {
                    if (label_addr.count(cmd.label)) {
                        throw TranslationError(fmt::format("Duplicated label `{}`", cmd.label), cmd.line);
                    } else {
                        label_addr[cmd.label] = curr_addr;
                    }
                }
                if (specs.count(cmd.command)) {
                    auto&& s = specs.at(cmd.command);
                    curr_addr += s->size();
                } else {
                    throw Error("Unknown command: " + cmd.command);
                }
            }
            return label_addr;
        }
    };
}