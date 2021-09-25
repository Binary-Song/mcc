#pragma once 

#include <boost/regex.hpp>
#include <boost/range/adaptor/indexed.hpp>

#include "common/precompiled.hpp"

namespace mcc {

    struct Token
    {
        int id;
        String lexeme;
        int line;
        int column;
        std::vector<String> submatches;

#ifndef NDEBUG
        String name;
#endif

        Token(int id = -1, String lexeme = "", int line = 0, int column = 0, std::vector<String> submatches = {})
        {
            this->id = id;
            this->lexeme = lexeme;
            this->line = line;
            this->column = column;
            this->submatches = std::move(submatches);
        }
    };

    class Tokenizer
    {
    public:

        struct NoMatchException
        {
            int line = 0;
            int column = 0;

            NoMatchException(int line = 0, int column = 0)
            {
                this->line = line;
                this->column = column;
            }

            String Message() const
            {
                return fmt::format("Unknown character at line {}, column {}.", line, column);
            }
        };

        VAR(TYPE(Vector<Tuple<String, Regex>>), patterns);
        VAR(TYPE(Set<String>), ignored_tokens);

        /// 词法分析
        /// @exception NoMatchException 无法与任何规则匹配。
        auto tokenize(std::string_view code) -> Vector<Token>
        {
            namespace b = boost;

            Vector<Token> tokens;
            auto&& begin = code.begin();
            auto&& end = code.end();

            int line = 1, col = 1;
            // 遍历字符
            for (auto&& current = begin; current != end; ++current) {
                // 缓存 token id到名称的映射
                Map<String, int> ids = get_token_ids();
#ifndef NDEBUG
                Map<int, String> names = get_token_names();
#endif  
                // 遍历规则
                bool found_matching_pattern = false;
                for (auto&& [token_name, regex] : patterns) {
                    auto matches = b::match_results<std::string_view::const_iterator>();
                    // 如果找到匹配
                    if (b::regex_search(current, end, matches, regex, b::regex_constants::match_continuous)) {
                        auto match_begin = matches[0].first;
                        auto match_end = matches[0].second;

                        int id = ids[token_name];
                        auto lexeme = String(match_begin, match_end);

                        if (!ignored_tokens.count(token_name)) {
                            Token token = Token(id, lexeme, line, col);
#ifndef NDEBUG
                            token.name = names[id];
#endif  
                            for (int i = 1; i < matches.size(); i++) {
                                token.submatches.push_back(String(matches[i].first, matches[i].second));
                            }
                            tokens.push_back(token);
                        }

                        if (lexeme.find('\n') != -1) {
                            line++;
                            col = 0;
                        } else {
                            col += lexeme.size();
                        }
                        found_matching_pattern = true;
                        break; // 从遍历规则中break出去
                    }
                }
                // 如果没找到匹配 
                if (!found_matching_pattern) {
                    throw NoMatchException(line, col);
                }
            }
            return tokens;
        }

        auto get_token_ids() const -> Map<String, int>
        {
            Map<String, int> map;
            for (auto&& [name, _] : patterns) {
                if (map.count(name) == 0) {
                    map[name] = (int) map.size();
                }
            }
            return map;
        }

        auto get_token_names() const -> Map<int, String>
        {
            Map<int, String> map;
            Set<String> set;
            for (auto&& [name, _] : patterns) {
                if (set.count(name) == 0) {
                    map[(int) map.size()] = name;
                    set.insert(name);
                }
            }
            return map;
        }
    };
}

