#include <iostream>
#include "Include/Generator.h"

int main(int argc, char** argv) {

    const char* usage = \
    "Usage: \n"\
    "./simplemdconverter <source_file> <converted_html_file>\n\n";

    std::string inp_file_name;
    std::string out_file_name;

    auto set_args = [&]() {
        if(argc != 3) {
            std::cerr << "Incorrect number of arguments supplied" << std::endl;
            std::cout << usage;
            exit(1);
        }
        FILE* check = fopen(argv[1], "r");
        if(check == nullptr) {
            std::cerr << "Source file cannot be read" << std::endl;
            std::cout << usage;
            fclose(check);
            exit(1);
        }
        fclose(check);
        inp_file_name = argv[1];
        out_file_name = argv[2];
    };
    set_args();

    simplemdconverter::Generator::convert(inp_file_name, out_file_name);
}