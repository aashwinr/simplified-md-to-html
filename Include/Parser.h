// Created by Ashwin Rohit on 5/2/22.

#ifndef PROPERMDTRANSLATER_PARSER_H
#define PROPERMDTRANSLATER_PARSER_H

#include <utility>
#include <vector>
#include "Tokenizer.h"

namespace simplemdconverter {

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
        ListItem,
        QuoteItem,
//      Link, Fixme: Parse links and images
//      Images,
    };

    struct ParseUnit {
        ParseUnitKind m_kind;
        string m_contents;
        bool m_is_terminating = false;
        explicit ParseUnit(ParseUnitKind kind, string contents = ""):
            m_kind{kind},
            m_contents{std::move(contents)}
            {}
    };

    class Parser {

    public:
    private:
        vector<Token> m_tokenlist;
        vector<ParseUnit> m_parse_unit_list;
        size_t m_context;
        uint8_t m_current_compound_depth;
        size_t m_pos;

    public:
        explicit Parser(vector<Token> tokenlist): m_tokenlist(std::move(tokenlist)), m_context(0), m_current_compound_depth(0), m_pos(0) {
            this->generate_parse_unit_list();
        }
        vector<ParseUnit> parse();

    private:
        ParseUnit& next();
        ParseUnit& consume();
        vector<ParseUnit> consume_while(const function<bool(const ParseUnit&)> &condition);
        inline vector<ParseUnit> consume_all_parse_unit_kind(ParseUnitKind kind);
        [[nodiscard]] inline bool end();
        inline static ParseUnitKind get_unit_kind(const Token &token);
        inline void set_context(ParseUnitKind kind);
        inline void unset_context(ParseUnitKind kind);
        [[nodiscard]] inline bool check_context(ParseUnitKind kind) const;

        ParseUnit& parse_uncompounded(ParseUnitKind kind);
        void generate_parse_unit_list();
        vector <ParseUnit> parse_text_ignore_newline();
        vector<ParseUnit> parse_text();

        // Uncompounded Types
        void terminate_headings(vector<ParseUnit>&);
        ParseUnit parse_heading();
        ParseUnit parse_bold();
        ParseUnit parse_italics();
        ParseUnit parse_code();
        ParseUnit parse_strikethrough();

        // Compunded Types
        vector <ParseUnit> parse_compounded(ParseUnitKind kind);
        vector <ParseUnit> parse_compounded_subunit(ParseUnitKind kind);
        static uint8_t get_compunded_unit_depth(const ParseUnit &compounded_unit);
        vector<ParseUnit> parse_list();
        vector<ParseUnit> parse_list_item();
        vector<ParseUnit> parse_quote();
        vector <ParseUnit> parse_quote_item();
        ParseUnit parse_newline();

    };
}

#endif //PROPERMDTRANSLATER_PARSER_H



























































































