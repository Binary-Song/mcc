#include "common/utility/functions/io.hpp"
#include <string>
#include <fmt/core.h>
#include <fmt/os.h>
#include <cassert>
using namespace mcc_utilites;
int main()
{
    OutputFile file;
    file.open(R"(D:\Projects\MyComputerCompilerSuite\mcc\runspace\asm\out.bin)");
    std::vector<unsigned char> buff = { 0x00, 0x01, 0x02, 0xFF, 0xEE, 0xCC, '\n' };
    file.write(buff);
    file.write(char(1));
}