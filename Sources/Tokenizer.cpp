/**
 * Created by Ashwin Rohit Alagiri Rajan
 */

#include "../Include/Tokenizer.h"

namespace simpleconv {

    using namespace std;
    vector<Token> Tokenizer::tokenize() {
        vector<Token> tokenList;
        while(!this->eof()) {
            switch (this->next()) {
                case '\n': {
                    tokenList.push_back(this->tokenize_newline());
                    break;
                }
                case '#':
                case '-':
                case '>':
                case '*':
                case '/':
                case '`':
                case '~':
//                case '(': Fixme: tokenize links and images
//                case ')':
//                case '[':
//                case ']':
                            {
                    tokenList.push_back(this->tokenize_specifier());
                    break;
                }
                default: {
                    tokenList.push_back(this->tokenize_text());
                    break;
                }
            }
        }
        return tokenList;
    }

    char Tokenizer::next() {
        return this->m_contents[this->m_pos];
    }

    char Tokenizer::consume() {
        char next = this->next();
        this->m_pos++;
        return next;
    }

    string Tokenizer::consume_while(const function<bool(char)>& condition) {
        string consumed;
        while(!this->eof() && condition(this->next()))
            consumed.push_back(this->consume());
        return consumed;
    }

    string Tokenizer::consume_whitespace() {
        return this->consume_while([](char x){
            return (bool)(isspace(x));
        });
    }

    string Tokenizer::consume_all_consecutive(char x) {
        return this->consume_while([x](char ch){ return ch == x; });
    }

    bool Tokenizer::eof() {
        return this->m_pos >= this->m_contents.size();
    }

    Token Tokenizer::tokenize_newline() {
        this->consume();
        return {
            TokenKind::Newline,
            "\n",
        };
    }

    Token Tokenizer::tokenize_specifier() {
        char specifier = this->next();
        Token ret =  {
            TokenKind::Specifier,
            this->consume_all_consecutive(specifier),
        };
        this->consume_whitespace();
        return ret;
    }

    Token Tokenizer::tokenize_text() {
        Token text_token;
        text_token.m_kind = TokenKind::Text;
        while(!this->eof()) {
            switch (this->next()) {
                case '\n':
                case '*':
                case '~':
                case '`':
                case '/':
                    return text_token;
                default:
                    text_token.m_contents.push_back(this->consume());
            }
        }
        return text_token;
    }


}