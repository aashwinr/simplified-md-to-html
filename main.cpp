#include "Include/Tokenizer.h"
#include "Include/Parser.h"
#include <string>

int main() {
    std::string contents = "# Hello this is a heading\n ## This is another heading\n";
    simpleconv::Tokenizer tokenizer(contents);
    auto tokens = tokenizer.tokenize();
    simpleconv::Parser parser(tokens);
    auto ret = parser.parse();
}