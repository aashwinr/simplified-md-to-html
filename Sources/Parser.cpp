//
// Created by Ashwin Rohit on 5/2/22.
//

#include "../Include/Parser.h"
#include <iostream>

template <typename T>
inline void push_back_all(std::vector<T> &dest, const std::vector<T> &src) {
    dest.insert(dest.end(), src.begin(), src.end());
}

namespace simpleconv {

    using namespace std;

    void Parser::generate_parse_unit_list() {
        for(const auto& i: this->m_tokenlist) {
            this->m_parse_unit_list.emplace_back(
                    Parser::get_unit_kind(i),
                    i.m_contents
            );
        }
    }

    ParseUnit& Parser::next() {
        return this->m_parse_unit_list[this->m_pos];
    }

    ParseUnit& Parser::consume() {
        ParseUnit& next = this->next();
        this->m_pos++;
        return next;
    }

    vector<ParseUnit> Parser::consume_while(const function<bool(const ParseUnit&)> &condition) {
        vector<ParseUnit> p_unit_list;
        while(!this->end() && condition(this->next())) {
            p_unit_list.push_back(this->consume());
        }
        return p_unit_list;
    }

    vector<ParseUnit> Parser::parse() {
        vector<ParseUnit> specifier_list;
        while(!this->end()) {
            switch (this->next().m_kind) {
                case ParseUnitKind::Heading:
                    specifier_list.push_back(this->parse_heading());
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
                    specifier_list.push_back(this->parse_text());
                    break;
                case ParseUnitKind::Newline:
                    this->parse_newline();
                    break;
                case ParseUnitKind::Invalid:
                    break;
                default:
                    cout << "Invalid case encountered" << endl;
            }
        }
        return specifier_list;
    }

    ParseUnit Parser::parse_text() {
        return this->consume();
    }

    ParseUnit Parser::parse_heading() {

        // Init
        ParseUnit heading_unit(ParseUnitKind::Heading, this->next().m_contents);
        this->consume_while([](const simpleconv::ParseUnit& p_unit){ return p_unit.m_kind == ParseUnitKind::Heading; });

        // Context Handling
        if(this->check_context(ParseUnitKind::Heading)) {
            this->unset_context(ParseUnitKind::Heading);
            return ParseUnit(ParseUnitKind::Invalid, "");
        }
        this->set_context(ParseUnitKind::Heading);

        while(!this->end()) {
            switch(this->next().m_kind) {

                case ParseUnitKind::Newline:
                    this->unset_context(ParseUnitKind::Heading);
                    this->parse_newline();
                    return heading_unit;

                case ParseUnitKind::Heading:
                    this->parse_heading();
                    return heading_unit;

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
                    heading_unit.m_subunits.push_back(this->parse_text());

            }
        }
        this->unset_context(ParseUnitKind::Heading);
        return heading_unit;
    }

    ParseUnit Parser::parse_list() {
        return ParseUnit(ParseUnitKind::Invalid, {});
    }

    ParseUnit Parser::parse_quote() {
        return ParseUnit(ParseUnitKind::Invalid, {});
    }

    ParseUnit Parser::parse_bold() {
        return ParseUnit(ParseUnitKind::Invalid, {});
    }

    ParseUnit Parser::parse_italics() {
        return ParseUnit(ParseUnitKind::Invalid, {});
    }

    ParseUnit Parser::parse_code() {
        return ParseUnit(ParseUnitKind::Invalid, {});
    }

    ParseUnit Parser::parse_strikethrough() {
        return ParseUnit(ParseUnitKind::Invalid, {});
    }

    ParseUnit Parser::parse_newline() {
        this->consume();
        return ParseUnit(ParseUnitKind::Newline, "\n");
    }

    bool Parser::end() {
        return this->m_pos >= this->m_parse_unit_list.size();
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