#pragma once

#include <iostream>
#include <boost/regex.hpp>
#include <fmt/core.h>

#include "common/frontend/tokenizer.hpp"
#include "common/precompiled.hpp"
#include "common/utility/functions/string_to_int.hpp"

namespace mcc {

    static const String token_address = "addr";
    static const String token_property = "prop";
    static const String token_flag = "flag";
    static const String token_separator = "sep";
    static const String token_space = "sp";
    static const String token_error = "err";

    struct Property
    {
        std::string key;
        std::string value;

        Property() = default;
        Property(std::string_view key, std::string_view value)
            :key(key), value(value)
        {}
    };

    struct Command
    {
        std::vector<std::string> flags;
        std::vector<Property> props;
        int row = 0;
        int line = 0;
        int column = 0;

        Command() = default;

        Command(std::vector<Token> const& flag_tokens, int row)
        {
            for (auto&& flag_token : flag_tokens) {
                this->flags.push_back(flag_token.lexeme);
                this->line = flag_token.line;
                this->column = flag_token.column;
            }
            this->row = row;
            this->line = line;
        }
    };

    struct CommandAddressConflict
    {
        int line = 0;
        int column = 0;
        int row_in_conflict = 0;
        CommandAddressConflict(int line, int column, int row_in_conflict)
        {
            this->line = line;
            this->column = column;
            this->row_in_conflict = row_in_conflict;
        }
        String what() const
        {
            return fmt::format("Unable to fit command into row {}"
                " because it is already occupied by a previous command.", row_in_conflict);
        }
    };

    /// 将token组合成指令。
    /// @exception CommandAddressConflict 指令地址冲突，用户试图将多个指令放在同一行时就会出现此问题。
    Vector<Command> parse(Tokenizer const& t, Vector<Token> const& tokens)
    { 
        Set<int> row_numbers;
        Vector<Command> commands;

        int row_number = 0;
        Vector<Token> flag_tokens;
        Vector<Token> prop_tokens;

        for (auto&& token : tokens) {
            if (token.id == t.to_id(token_address)) {
                row_number = string_to_int(token.submatches[0]);
            } else if (token.id == t.to_id(token_flag)) {
                // add flag
                flag_tokens.push_back(token);
            } else if (token.id == t.to_id(token_property)) {
                // add prop
                prop_tokens.push_back(token);
            } else if (token.id == t.to_id(token_separator)) {
                if (flag_tokens.size()) {
                    // new command
                    if (row_numbers.count(row_number)) {
                        throw CommandAddressConflict(token.line, token.column, row_number);
                    } else {
                        row_numbers.insert(row_number);
                        commands.push_back(Command(flag_tokens, row_number));
                        row_number++;
                        flag_tokens.clear();
                    }
                }
            }
        }
        return commands;
    }
}
