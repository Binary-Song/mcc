#pragma once

#include <iostream>
#include <boost/regex.hpp>
#include <fmt/core.h>
#include <boost/algorithm/string/predicate.hpp>

#include "common/frontend/tokenizer.hpp"
#include "common/precompiled.hpp"
#include "common/utility/functions/string_to_int.hpp"
#include "asm/error.hpp"
namespace mcc {

    static const String token_label = "label";
    static const String token_separator = "separator";
    static const String token_space = "space";
    static const String token_comma = "comma";
    static const String token_raw = "raw";
    static const String token_label_ref = "label_ref";
    static const String token_arg_or_cmd = "arg_or_cmd";

    struct Command
    {
        String label = "";
        String command;
        Vector<String> args;
        int line = 0;

        Command() = default;

        Command(Vector<Token> const& tokens)
        {
            for (auto&& token : tokens) {
                this->args.push_back(token.lexeme);
                this->line = token.line;
            }
        }
    };

    struct Program
    {
        Vector<Command> commands;

        Map<String, Vector<int>> label_refs;
    };

    class Frontend
    {
    public:

        Frontend()
        {
            tokenizer = Tokenizer().set_patterns(patterns).set_ignored_tokens({ token_space, token_comma });
        }

        Vector<Token> scan(StringView code)
        {
            return tokenizer.tokenize(code);
        }



        /// 将token组合成指令。 
        Vector<Command> parse(Tokenizer const& t, Vector<Token> const& tokens)
        {
            Vector<Command> commands;
            String label;
            Vector<Token> cmd_tokens;

            for (auto&& token : tokens) {
                if (token.id == t.to_id(token_label)) {
                    if (label != "") {
                        throw TranslationError("Cannot specify more than 1 label on a command."
                            , token.line, token.column, token.lexeme.size());
                    }
                    if (cmd_tokens.size()) {
                        throw TranslationError("Labels should not appear in the middle of a command."
                            , token.line, token.column, token.lexeme.size());
                    }
                    label = token.submatches[0];
                } else if (token.id == t.to_id(token_arg_or_cmd)) {
                    // add flag
                    cmd_tokens.push_back(token);
                } else if (token.id == t.to_id(token_raw)) {
                    // add prop
                    cmd_tokens.push_back(token);
                } else if (token.id == t.to_id(token_separator)) {
                    if (cmd_tokens.size()) {
                        // new command
                        Command cmd;
                        if (boost::starts_with(cmd_tokens[0].lexeme, "0x")) {
                            cmd.command = "raw";
                        } else {
                            cmd.command = cmd_tokens[0].lexeme;
                        }

                        for (int i = 1; i < cmd_tokens.size(); i++) {
                            cmd.args.push_back(cmd_tokens[i].lexeme);
                        }
                        cmd.label = label;
                        cmd.line = token.line;

                        commands.push_back(cmd);
                        // clear states
                        cmd_tokens.clear();
                        label = "";
                    }
                }
            }
            return commands;
        }


        Tokenizer tokenizer;
    private:

        const Vector<Tuple<String, Regex>> patterns = {
            {String(token_label), Regex{"(\\.[0-9A-Za-z_]+):"}},
            {String(token_arg_or_cmd), Regex{"[a-zA-Z_][a-zA-Z_0-9]+"}},
            {String(token_label_ref), Regex{"\\.[0-9A-Za-z_]+"}},
            {String(token_raw), Regex{"0x[0-9A-Fa-f]{2}"}},
            {String(token_separator), Regex{"(#|;).*\n"}},
            {String(token_separator), Regex{"\n"}},
            {String(token_space), Regex{"(\\s+)"}},
            {String(token_comma), Regex{","}},
        };
    };


}
