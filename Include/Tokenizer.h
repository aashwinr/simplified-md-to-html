//
// Created by Ashwin Rohit on 5/1/22.
//

#ifndef PROPERMDTRANSLATER_TOKENIZER_H
#define PROPERMDTRANSLATER_TOKENIZER_H

#include <string>
#include <utility>
#include <vector>
#include <optional>
#include <functional>

namespace simpleconv {

    using namespace std;

    enum class TokenKind {
        Text,
        Specifier,
        Newline,
    };

    struct Token {
        TokenKind m_kind;
        string m_contents;
    };

    class Tokenizer {

    public:
    private:
        string m_contents;
        size_t m_pos;

    public:
        explicit Tokenizer(string source): m_contents(std::move(source)), m_pos(0) {};
        vector<Token> tokenize();

    private:
        // Peeking methods
        char next();
        char consume();
        string consume_while(const function<bool(char)>& condition);
        string consume_whitespace();
        string consume_all_consecutive(char x);
        inline bool eof();

        // Tokenizing methods
        Token tokenize_newline();
        Token tokenize_specifier();
        Token tokenize_text();

    };

}

#endif //PROPERMDTRANSLATER_TOKENIZER_H
