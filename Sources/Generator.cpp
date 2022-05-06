//
// Created by Zirconium  on 5/4/22.
//

#include "../Include/Generator.h"
#include <fstream>
#include <string>

std::string simpleconv::Generator::get_html_element(const simpleconv::ParseUnit &p_unit) {
    bool is_terminating = p_unit.m_is_terminating;
    switch (p_unit.m_kind) {
        case ParseUnitKind::Heading:
            return is_terminating ? "</h1>" : "<h1>";
        case ParseUnitKind::List:
            return is_terminating ? "</ul>" : "<ul>";
        case ParseUnitKind::Quote:
            return is_terminating ? "</div>" : "<div class=\"quote\">";
        case ParseUnitKind::Bold:
            return is_terminating ? "</strong>" : "<strong>";
        case ParseUnitKind::Italics:
            return is_terminating ? "</em>" : "<em>";
        case ParseUnitKind::Code:
            return is_terminating ? "</code>" : "<code>";
        case ParseUnitKind::Strikethrough:
            return is_terminating ? "</strirke>" : "<strike>";
        case ParseUnitKind::Text:
            return p_unit.m_contents;
        case ParseUnitKind::Newline:
            return "<br>";
        case ParseUnitKind::ListItem:
            return is_terminating ? "</li>" : "<li>";
        case ParseUnitKind::QuoteItem:
            return is_terminating ? "</section>" : "<section class=\"quote-item\">";
    }
}

std::string simpleconv::Generator::generate_html_body() {
    string output;
    for(const simpleconv::ParseUnit& p_unit: this->m_parse_unit_list) {
        output += (simpleconv::Generator::get_html_element(p_unit));
    }
    return output;
}

void simpleconv::Generator::convert(const std::string &source_file_path, const std::string &output_file_path) {

    string file_contents;
    string line;
    std::fstream source_file(source_file_path, std::fstream::in);
    std::fstream output_file(output_file_path, std::fstream::out);
    while(getline(source_file, line)) {
        file_contents += line + '\n';
    }

    simpleconv::Tokenizer tokenizer(file_contents);
    std::vector<Token> token_list = tokenizer.tokenize();

    simpleconv::Parser parser(token_list);
    std::vector<ParseUnit> parse_unit_list = parser.parse();

    simpleconv::Generator generator(parse_unit_list);
    string output = generator.generate_html_body();

    output_file << output;

}
