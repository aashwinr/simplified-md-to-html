//
// Created by Ashwin Rohit on 5/2/22.
//

#include "../Include/Parser.h"
#include <iostream>

template <typename T>
inline void push_back_all(std::vector<T> &dest, const std::vector<T> &src) {
    dest.insert(dest.end(), src.begin(), src.end());
}

namespace simplemdconverter {

    using namespace std;

    void Parser::generate_parse_unit_list() {
        for(const auto& token: this->m_tokenlist) {
            this->m_parse_unit_list.emplace_back(
                    Parser::get_unit_kind(token),
                    token.m_contents
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
                    push_back_all(specifier_list, this->parse_list());
                    break;
                case ParseUnitKind::Quote:
                    push_back_all(specifier_list, this->parse_quote());
                    break;
                case ParseUnitKind::Code:
                case ParseUnitKind::Bold:
                case ParseUnitKind::Italics:
                case ParseUnitKind::Strikethrough:
                case ParseUnitKind::Text:
                    push_back_all(specifier_list, this->parse_text());
                    break;
                case ParseUnitKind::Newline:
                    specifier_list.push_back(this->parse_newline());
                    break;
                default:
                    cout << "Invalid case encountered" << endl;
            }
        }
        return specifier_list;
    }

    vector<ParseUnit> Parser::parse_text() {
        this->set_context(ParseUnitKind::Text);
        vector<ParseUnit> ret_list;
        while(!this->end()) {
            switch(this->next().m_kind) {
                case ParseUnitKind::Bold:
                    ret_list.push_back(this->parse_bold());
                    break;
                case ParseUnitKind::Italics:
                    ret_list.push_back(this->parse_italics());
                    break;
                case ParseUnitKind::Code:
                    ret_list.push_back(this->parse_code());
                    break;
                case ParseUnitKind::Strikethrough:
                    ret_list.push_back(this->parse_strikethrough());
                    break;
                case ParseUnitKind::Newline:
                    this->terminate_headings(ret_list);
                    this->parse_newline();
                    this->unset_context(ParseUnitKind::Text);
                    return ret_list;
                case ParseUnitKind::Text:
                default:
                    ret_list.emplace_back(ParseUnitKind::Text, this->consume().m_contents);
            }
        }
        this->unset_context(ParseUnitKind::Text);
        return ret_list;
    }

    vector<ParseUnit> Parser::parse_text_ignore_newline() {
        this->set_context(ParseUnitKind::Text);
        vector<ParseUnit> ret_list;
        while(!this->end()) {
            switch(this->next().m_kind) {
                case ParseUnitKind::Bold:
                    ret_list.push_back(this->parse_bold());
                    break;
                case ParseUnitKind::Italics:
                    ret_list.push_back(this->parse_italics());
                    break;
                case ParseUnitKind::Code:
                    ret_list.push_back(this->parse_code());
                    break;
                case ParseUnitKind::Strikethrough:
                    ret_list.push_back(this->parse_strikethrough());
                    break;
                case ParseUnitKind::Newline:
                    this->terminate_headings(ret_list);
                    this->unset_context(ParseUnitKind::Text);
                    return ret_list;
                case ParseUnitKind::Text:
                default:
                    ret_list.emplace_back(ParseUnitKind::Text, this->consume().m_contents);
            }
        }
        this->unset_context(ParseUnitKind::Text);
        return ret_list;
    }

    ParseUnit& Parser::parse_uncompounded(ParseUnitKind kind) {
        ParseUnit& ret = this->consume();
        if(this->check_context(kind)) {
            unset_context(kind);
            ret.m_is_terminating = true;
            return ret;
        }
        set_context(kind);
        return ret;
    }

    void Parser::terminate_headings(vector<ParseUnit> &return_list) {
        if(this->check_context(ParseUnitKind::Heading)) {
            this->unset_context(ParseUnitKind::Heading);
            ParseUnit ret = ParseUnit(ParseUnitKind::Heading);
            ret.m_is_terminating = true;
            return_list.push_back(ret);
        }
    }

    ParseUnit Parser::parse_heading() {
        return this->parse_uncompounded(ParseUnitKind::Heading);
    }

    uint8_t Parser::get_compunded_unit_depth(const ParseUnit &compounded_unit) {
        return compounded_unit.m_contents.size();
    }

    vector<ParseUnit> Parser::parse_compounded(ParseUnitKind kind) {
        this->consume();
        this->m_current_compound_depth++;
        vector<ParseUnit> ret_list;
        ret_list.emplace_back(kind);
        vector<ParseUnit> subunits;

        bool break_outer_loop = false;
        ParseUnit terminating(kind);
        terminating.m_is_terminating = true;

        if(kind == ParseUnitKind::List) {
            push_back_all(ret_list, this->parse_list_item());
        } else {
            push_back_all(ret_list, this->parse_quote_item());
        }

        while(!this->end()) {
            switch(this->next().m_kind) {
                case ParseUnitKind::Heading:
                    subunits.push_back(this->parse_heading());
                    break;
                case ParseUnitKind::List: {
                    uint8_t compound_depth = Parser::get_compunded_unit_depth(this->next());
                    if (compound_depth != this->m_current_compound_depth) {
                        if (compound_depth < this->m_current_compound_depth) {
                            push_back_all(ret_list, subunits);
                            ret_list.push_back(terminating);
                            this->m_current_compound_depth--;
                            return ret_list;
                        }
                        push_back_all(subunits, this->parse_list());
                    } else {
                        this->consume();
                        push_back_all(subunits, this->parse_list_item());
                    }
                }
                    break;
                case ParseUnitKind::Quote: {
                    uint8_t compound_depth = Parser::get_compunded_unit_depth(this->next());
                    if (compound_depth != this->m_current_compound_depth) {
                        if (compound_depth < this->m_current_compound_depth) {
                            push_back_all(ret_list, subunits);
                            ret_list.push_back(terminating);
                            this->m_current_compound_depth--;
                            return ret_list;
                        }
                        push_back_all(subunits, this->parse_quote());
                    } else {
                        this->consume();
                        push_back_all(subunits, this->parse_quote_item());
                    }
                }
                    break;
                case ParseUnitKind::Newline:
                    this->parse_newline();
                    if(this->end() || this->next().m_kind != kind) {
                        break_outer_loop = true;
                        break;
                    }
                    break;
                case ParseUnitKind::Bold:
                case ParseUnitKind::Italics:
                case ParseUnitKind::Code:
                case ParseUnitKind::Strikethrough:
                case ParseUnitKind::Text:
                default:
                    push_back_all(subunits, this->parse_text());
                    break;
            }
            if(break_outer_loop) { break; }
        }
        push_back_all(ret_list, subunits);
        ret_list.push_back(terminating);
        this->m_current_compound_depth--;
        return ret_list;
    }

    vector<ParseUnit> Parser::parse_compounded_subunit(ParseUnitKind kind) {
        vector<ParseUnit> ret;
        ParseUnit beginning(kind);
        ParseUnit terminating(kind);
        terminating.m_is_terminating = true;
        ret.push_back(beginning);
        while(!this->end()) {
            switch (this->next().m_kind) {
                case ParseUnitKind::Heading:
                    ret.push_back(this->parse_heading());
                    break;
                case ParseUnitKind::List:
                    return this->parse_list();
                case ParseUnitKind::Quote:
                    return this->parse_quote();
                case ParseUnitKind::Newline:
                    this->parse_newline();
                    ret.push_back(terminating);
                    return ret;
                case ParseUnitKind::Bold:
                case ParseUnitKind::Italics:
                case ParseUnitKind::Code:
                case ParseUnitKind::Strikethrough:
                case ParseUnitKind::Text:
                default:
                    push_back_all(ret, this->parse_text_ignore_newline());
            }
        }
        ret.push_back(terminating);
        return ret;
    }

    vector<ParseUnit> Parser::parse_list() {
        return this->parse_compounded(ParseUnitKind::List);
    }

    vector<ParseUnit> Parser::parse_list_item() {
        return this->parse_compounded_subunit(ParseUnitKind::ListItem);
    }

    vector<ParseUnit> Parser::parse_quote() {
        return this->parse_compounded(ParseUnitKind::QuoteItem);
    }

    vector<ParseUnit> Parser::parse_quote_item() {
        return this->parse_compounded_subunit(ParseUnitKind::QuoteItem);
    }

    ParseUnit Parser::parse_bold() {
        return this->parse_uncompounded(ParseUnitKind::Bold);
    }

    ParseUnit Parser::parse_italics() {
        return this->parse_uncompounded(ParseUnitKind::Italics);
    }

    ParseUnit Parser::parse_code() {
        return this->parse_uncompounded(ParseUnitKind::Code);
    }

    ParseUnit Parser::parse_strikethrough() {
        return this->parse_uncompounded(ParseUnitKind::Strikethrough);
    }

    ParseUnit Parser::parse_newline() {
        return this->consume();
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

    vector<ParseUnit> Parser::consume_all_parse_unit_kind(ParseUnitKind kind) {
        return this->consume_while([=](const ParseUnit& p_unit){
            return p_unit.m_kind == kind;
        });
    }

}