//
// Created by Zirconium  on 5/4/22.
//

#ifndef PROPERMDTRANSLATER_GENERATOR_H
#define PROPERMDTRANSLATER_GENERATOR_H

#include <vector>
#include "../Include/Parser.h"

namespace simpleconv {
    using namespace std;
    class Generator {
    public:
        explicit Generator(const vector<simpleconv::ParseUnit> &p_unit_list): m_parse_unit_list{p_unit_list} {}
        string generate_html_body();
        static void convert(const string& source_file_path, const string& output_file_path);
    private:
        vector<simpleconv::ParseUnit> m_parse_unit_list;
        static string get_html_element(const simpleconv::ParseUnit &p_unit);
    };
}
#endif //PROPERMDTRANSLATER_GENERATOR_H
