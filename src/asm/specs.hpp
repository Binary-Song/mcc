#pragma once
 
namespace mcc{

#define COMMAND_SHARED_PTR(cmd) std::make_shared<cmd>(cmd())
#define COMMAND_SHARED_PTR_1(cmd,arg1) std::make_shared<cmd>(cmd(arg1))
#define COMMAND_SHARED_PTR_2(cmd,arg1,arg2) std::make_shared<cmd>(cmd(arg1,arg2))
#define COMMAND_SHARED_PTR_3(cmd,arg1,arg2,arg3) std::make_shared<cmd>(cmd(arg1,arg2,arg3))

    static inline Map<String, SharedPtr<CommandSpecs>> specs = {
        {"raw", COMMAND_SHARED_PTR(CommandRaw) },
        {"clr", COMMAND_SHARED_PTR(CommandClear) },
        {"add", COMMAND_SHARED_PTR(CommandAdd)},
        {"sub", COMMAND_SHARED_PTR(CommandSubtract)},
        {"mov", COMMAND_SHARED_PTR(CommandMove)},

        {"inc", COMMAND_SHARED_PTR(CommandIncrease)},
        {"dec", COMMAND_SHARED_PTR(CommandDecrease)},

        {"ld",  COMMAND_SHARED_PTR_1(CommandLoad, true)},
        {"ld&",  COMMAND_SHARED_PTR_1(CommandLoad, true)},
        {"ldrom", COMMAND_SHARED_PTR_1(CommandLoad, false)},
        {"ldrom&", COMMAND_SHARED_PTR_1(CommandLoad, false)},

        {"st",  COMMAND_SHARED_PTR(CommandStore)},
        {"st&",  COMMAND_SHARED_PTR(CommandStore)},
 
        {"bzc", COMMAND_SHARED_PTR_1(CommandBranchZeroCarry, true)},
        {"bzc&", COMMAND_SHARED_PTR_1(CommandBranchZeroCarry, true)},
        {"bzcrom", COMMAND_SHARED_PTR_1(CommandBranchZeroCarry, false)},
        {"bzcrom&", COMMAND_SHARED_PTR_1(CommandBranchZeroCarry, false)},

        {"jmp", COMMAND_SHARED_PTR_1(CommandJump, true)},
        {"jmp&", COMMAND_SHARED_PTR_1(CommandJump, true)},
        {"jmprom", COMMAND_SHARED_PTR_1(CommandJump, false)},
        {"jmprom&", COMMAND_SHARED_PTR_1(CommandJump, false)}, 
    };
#undef COMMAND_SHARED_PTR
#undef COMMAND_SHARED_PTR_1
#undef COMMAND_SHARED_PTR_2
#undef COMMAND_SHARED_PTR_3

}
