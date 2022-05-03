//
// Created by Ashwin Rohit on 5/2/22.
//

#include "../Include/Parser.h"

template <typename T>
inline void push_back_all(std::vector<T> &dest, const std::vector<T> &src) {
    dest.insert(dest.begin(), src.begin(), src.end());
}

namespace simpleconv {

    using namespace std;

    Token& Parser::next() {
        return this->m_tokenlist[this->m_pos];
    }

    Token &Parser::consume() {
        Token& next = this->next();
        this->m_pos++;
        return next;
    }

    vector<Token> Parser::consume_while(const function<bool(const Token&)> &condition) {
        vector<Token> tokenlist;
        while(!this->end() && condition(this->next())) {
            tokenlist.push_back(this->consume());
        }
        return tokenlist;
    }

    vector<ParseUnit> Parser::parse() {
        vector<ParseUnit> specifier_list;
        while(!this->end()) {
            ParseUnitKind next_specifier_kind = Parser::get_unit_kind(this->next());
            switch (next_specifier_kind) {
                case ParseUnitKind::Heading:
                    break;
                case ParseUnitKind::List:
                    break;
                case ParseUnitKind::Quote:
                    break;
                case ParseUnitKind::Code:
                    break;
                case ParseUnitKind::Bold:
                case ParseUnitKind::Italics:
                case ParseUnitKind::Strikethrough:
                case ParseUnitKind::Text:
                    break;
                case ParseUnitKind::Newline:
                    break;
            }
        }
    }

    vector<ParseUnit> Parser::parse_text() {

    }

    ParseUnit Parser::parse_heading() {

        this->consume_while([](const Token& token){ return Parser::get_unit_kind(token) == ParseUnitKind::Heading; });
        if(this->check_context(ParseUnitKind::Heading)) {
            this->unset_context(ParseUnitKind::Heading);
            return ParseUnit(ParseUnitKind::Invalid, "");
        }
        this->set_context(ParseUnitKind::Heading);

        ParseUnit heading_unit(ParseUnitKind::Heading, this->next().m_contents);

        while(!this->end()) {

            ParseUnitKind current_token_kind = get_unit_kind(this->next());
            if(current_token_kind == ParseUnitKind::Newline) {
                this->consume();
                return heading_unit;
            } else if(current_token_kind == ParseUnitKind::Heading){
                this->parse_heading();
                return heading_unit;
            }

            switch(current_token_kind) {
                case ParseUnitKind::Bold:
                    heading_unit.m_subunits.push_back(this->parse_bold());
                    break;
                case ParseUnitKind::Italics:
                    heading_unit.m_subunits.push_back(this->parse_italics());
                    break;
                case ParseUnitKind::Strikethrough:
                    heading_unit.m_subunits.push_back(this->parse_strikethrough());
                    break;
                default:
                    push_back_all(heading_unit.m_subunits, this->parse_text());
                    break;
            }

        }
        return heading_unit;
    }

    ParseUnit Parser::parse_list() {
        return {};
    }

    ParseUnit Parser::parse_quote() {
        return {};
    }

    ParseUnit Parser::parse_bold() {
        return {};
    }

    ParseUnit Parser::parse_italics() {
        return {};
    }

    ParseUnit Parser::parse_code() {
        return {};
    }

    ParseUnit Parser::parse_strikethrough() {
        return {};
    }

    ParseUnit Parser::parse_newline() {
        return {};
    }

    bool Parser::end() {
        return this->m_pos >= this->m_tokenlist.size();
    }

    ParseUnitKind Parser::get_unit_kind(const Token &token) {
        switch (token.m_kind) {
            case TokenKind::Text:
                return ParseUnitKind::Text;
            case TokenKind::Specifier:
                switch (token.m_contents.at(0)) {
                    case '#':
                        return ParseUnitKind::Heading;
                    case '-':
                        return ParseUnitKind::List;
                    case '>':
                        return ParseUnitKind::Quote;
                    case '*':
                        return ParseUnitKind::Bold;
                    case '/':
                        return ParseUnitKind::Italics;
                    case '`':
                        return ParseUnitKind::Code;
                    case '~':
                        return ParseUnitKind::Strikethrough;
                }
            case TokenKind::Newline:
                return ParseUnitKind::Newline;
        }
    }

    void Parser::set_context(ParseUnitKind kind) {
        this->m_context |= (size_t)kind;
    }

    void Parser::unset_context(ParseUnitKind kind) {
        this->m_context &= ~((size_t)kind);
    }

    bool Parser::check_context(ParseUnitKind kind) const {
        return (bool)(this->m_context & (size_t)kind);
    }


}