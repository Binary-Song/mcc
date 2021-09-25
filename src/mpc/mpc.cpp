#include <iostream>
#include "common/frontend/tokenizer.hpp"
#include "common/precompiled.hpp"
#include <boost/regex.hpp>
#include <fmt/core.h>
int main(int argc, char** argv)
{
    using namespace mcc;
    Vector<Tuple<String, Regex>> patterns = {
          {"addr16", Regex{"0x[0-9A-Fa-f]+:"}},
          {"addr2", Regex{"0b[0-1]+:"}},
          {"addr10", Regex{"[0-9]+:"}},
          {"flg", Regex{"[a-zA-Z_0-9]+"}},
          {"prop", Regex{"([a-zA-Z_0-9]+)=([a-zA-Z0-9+-]+)]"}},
          {"sep", Regex{"(#|;)[^\n]*(\n|$)"}},
          {"sep", Regex{"\n"}},
          {"space", Regex{"\\s"}}
    };



    try {
        String code;
        code = read_file_as_string(argv[1]);
        auto tokenizer = Tokenizer().set_patterns(patterns).set_ignored_tokens({ "space" });
        tokenizer.tokenize(code);

    }
    catch (FileNotOpenedException& except) {
        std::cerr << fmt::format("Error: {}", except.Message());
        return;
    }
    catch (Tokenizer::NoMatchException const& exception) {
        std::cerr << "Error: " << exception.Message();
    }
}