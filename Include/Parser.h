// Created by Ashwin Rohit on 5/2/22.

#ifndef PROPERMDTRANSLATER_PARSER_H
#define PROPERMDTRANSLATER_PARSER_H

#include <utility>
#include <vector>
#include "Tokenizer.h"

namespace simpleconv {

    using namespace std;

    enum class ParseUnitKind {
        Heading = 2,
        List = 4,
        Quote = 8,
        Bold = 16,
        Italics = 32,
        Code = 64,
        Strikethrough = 128,
        Text = 256,
        Newline = 512,
        Invalid,
//      Link, Fixme: Parse links and images
//      Images,
    };

    struct ParseUnit {
        ParseUnitKind m_kind;
        string m_contents;
        vector<ParseUnit> m_subunits;

        explicit ParseUnit(ParseUnitKind kind, string contents):
            m_kind{kind},
            m_contents{std::move(contents)},
            m_subunits{{}} {}
    };


    class Parser {
    public:
    private:
        vector<Token> m_tokenlist;
        size_t m_context;
        size_t m_pos;

    public:
        explicit Parser(vector<Token> tokenlist): m_tokenlist(std::move(tokenlist)), m_context(0), m_pos(0) {}
        vector<ParseUnit> parse();

    private:
        vector<ParseUnit> parse_text();
        ParseUnit parse_heading();
        ParseUnit parse_list();
        ParseUnit parse_quote();
        ParseUnit parse_bold();
        ParseUnit parse_italics();
        ParseUnit parse_code();
        ParseUnit parse_strikethrough();
        ParseUnit parse_newline();
        Token& next();
        Token& consume();
        vector<Token> consume_while(const function<bool(const Token&)> &condition);
        inline bool end();
        inline static ParseUnitKind get_unit_kind(const Token &token);
        inline void set_context(ParseUnitKind kind);
        inline void unset_context(ParseUnitKind kind);
        inline bool check_context(ParseUnitKind kind) const;
    };
}

#endif //PROPERMDTRANSLATER_PARSER_H
