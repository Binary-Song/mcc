#include "common/utility/functions/string_to_int.hpp"
#include <string>
#include <fmt/core.h>
#include <fmt/os.h>
#include <cassert>
using namespace mcc_utilites;
int main()
{
    for (int i = 10; i < 1000; i++) {
        fmt::print("Testing... {}/1000 \n", i);
        int conv_x = string_to_int(fmt::format("0x{0:x}", i));
        assert(conv_x == i);
        int conv_b = string_to_int(fmt::format("0b{0:b}", i));
        assert(conv_x == i);
        int conv_o = string_to_int(fmt::format("0o{0:o}", i));
        assert(conv_o == i);
        int nconv_x = string_to_int(fmt::format("-0x{0:x}", i));
        assert(nconv_x == -i);
        int nconv_b = string_to_int(fmt::format("-0b{0:b}", i));
        assert(nconv_x == -i);
        int nconv_o = string_to_int(fmt::format("-0o{0:o}", i));
        assert(nconv_o == -i);
    }

    fmt::print("All tests passed.\n");
 
}