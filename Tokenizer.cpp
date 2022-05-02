/**
 * Created by Ashwin Rohit Alagiri Rajan
 */

#include "Tokenizer.h"

namespace Tokenizer {

    using namespace std;
    vector<Token> Tokenizer::tokenize() {
        vector<Token> token_list;
        while(!this->eof()) {
            switch (this->next()) {
                case '\n': {
                    token_list.push_back(this->tokenize_newline());
                    break;
                }
                case '#':
                case '-':
                case '>': {
                    token_list.push_back(this->tokenize_specifier());
                    break;
                }
                case '*':
                case '/':
                case '`':
                case '~': {
                    vector<Token> paired_tokens = this->tokenize_paired_specifier();
                    token_list.insert(token_list.end(), paired_tokens.begin(), paired_tokens.end());
                    break;
                }
                default: {
                    token_list.push_back(this->tokenize_text());
                    break;
                }
            }
        }
        return token_list;
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
        return consume_while([](char x) -> bool{
            return isspace(x);
        });
    }

    string Tokenizer::consume_all_consecutive(char x) {
        return this->consume_while([=](char ch){ return ch == x; });
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
        return {
            TokenKind::Specifier,
            this->consume_all_consecutive(specifier),
        };
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

    vector<Token> Tokenizer::tokenize_paired_specifier() {
        vector<Token> token_list;
        char current_specifier = this->next();
        token_list.push_back(this->tokenize_specifier());
        while(!this->eof()) {
            Token internal_text_token = this->tokenize_text();
            token_list.push_back(internal_text_token);
            if(Tokenizer::is_specifier(this->next()) && this->next() != current_specifier) {
                vector<Token> internal_specifier_tokens = this->tokenize_paired_specifier();
                token_list.insert(token_list.end(), internal_specifier_tokens.begin(), internal_specifier_tokens.end());
            } else if(this->next() == '\n') {
                return token_list;
            } else {
                token_list.push_back(this->tokenize_specifier());
            }
        }
        return token_list;
    }

    bool Tokenizer::is_specifier(char x) {
        switch (x) {
            case '#':
            case '-':
            case '>':
            case '*':
            case '/':
            case '`':
            case '~': return true;
            default: return false;
        }
    }

}