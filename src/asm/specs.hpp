#pragma once
 
namespace mcc{

#define COMMAND_SHARED_PTR(cmd) std::make_shared<cmd>(cmd())
    static inline Map<String, SharedPtr<CommandSpecs>> specs = {
        {"raw", COMMAND_SHARED_PTR(CommandRaw) },
        {"clr", COMMAND_SHARED_PTR(CommandClear) },
        {"add", COMMAND_SHARED_PTR(CommandAdd)},
        {"sub", COMMAND_SHARED_PTR(CommandSubtract)},
        {"mov", COMMAND_SHARED_PTR(CommandMove)},

        {"inc", COMMAND_SHARED_PTR(CommandIncrease)},
        {"dec", COMMAND_SHARED_PTR(CommandDecrease)},

        {"ld",  COMMAND_SHARED_PTR(CommandLoad)},
        {"ld&",  COMMAND_SHARED_PTR(CommandLoad)},
        {"st",  COMMAND_SHARED_PTR(CommandStore)},
        {"st&",  COMMAND_SHARED_PTR(CommandStore)},
        {"bzc", COMMAND_SHARED_PTR(CommandBranchZeroCarry)},
        {"bzc&", COMMAND_SHARED_PTR(CommandBranchZeroCarry)},
        {"ldrom", COMMAND_SHARED_PTR(CommandLoadRom)},
        {"ldrom&", COMMAND_SHARED_PTR(CommandLoadRom)},
    };
#undef COMMAND_SHARED_PTR

}
