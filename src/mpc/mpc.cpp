#include <iostream>
#include <boost/regex.hpp>
#include <fmt/core.h>
#include <filesystem>

#include "common/frontend/tokenizer.hpp"
#include "common/precompiled.hpp"
#include "common/utility/functions/string_to_int.hpp"
#include "common/error_visualizer.hpp"
#include "common/hasher.hpp"
#include "mpc/frontend.hpp"
#include "mpc/backend.hpp"


int main(int argc, char** argv)
{
    using namespace mcc;

    Vector<Tuple<String, Regex>> patterns = {
          {String(token_address), Regex{"((0[box])?[0-9A-Fa-f]+):"}},
          {String(token_property), Regex{"[a-zA-Z_][a-zA-Z_0-9]+=[+-]?(0[box])?[0-9A-Fa-f]+"}},
          {String(token_flag), Regex{"[a-zA-Z_][a-zA-Z_0-9]+"}},
          {String(token_separator), Regex{"(#|;).*\n"}},
          {String(token_separator), Regex{"\n"}},
          {String(token_space), Regex{"\\s+"}},
    };

    if (argc < 2) {
        std::cerr << "Usage: mpc <path> [-o <dir>]\n";
        return 1;
    }

    namespace fs = std::filesystem;


    mcc::ErrorVisualizer errprint;
    String code;
    fs::path input;
    fs::path output_dir;

    try {

        input = argv[1];
        output_dir = argc >= 3 ? fs::path(argv[2]) : input / "..";

        input = fs::canonical(input); // fs::canonical 要求该路径必须存在！
        output_dir = fs::canonical(output_dir);
        auto output_digest = output_dir / "digest.sha1";

        code = file_to_string(input) + "\n";

        String last_hash = file_to_string_with_fallback(output_digest, "");
        String this_hash = Hasher().compute_hash(code);
        String status;

        Hasher hash;
        if (this_hash != last_hash) {
            status = "regenerated";
            errprint.fit(code);
            auto tokenizer = Tokenizer().set_patterns(patterns).set_ignored_tokens({ token_space });
            auto tokens = tokenizer.tokenize(code);
            auto cmds = parse(tokenizer, tokens);
            auto backend = MyComputerBackend(output_dir.string());
            backend.write_output(cmds);
        } else {
            status = "skipped";
        }

        overwrite_file(this_hash, output_digest);

        fmt::print(fg(fmt::color::light_sea_green), "Build successful ({}). Artifacts stored in {} .\n", status, output_dir.string());
    }
    catch (IOError const& e) {
        errprint.print_error(e.what(), e.path);
        return 1;
    }
    catch (fs::filesystem_error const& e) {
        errprint.print_error("Path does not exist.", e.path1().string());
    }
    catch (Tokenizer::NoMatchException const& e) {
        errprint.print_error(e.what(), input.string(), code, e.line, e.column);
        return 1;
    }
    catch (CommandAddressConflict const& e) {
        errprint.print_error(e.what(), input.string(), code, e.line);
    }
    return 0;
}