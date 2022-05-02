#include "Include/Tokenizer.h"
#include <string>

int main() {
    std::string contents = "# Hello this is a heading\n- This is a *stylized* list\n";
    simpleconv::Tokenizer tokenizer(contents);
    auto tokens = tokenizer.tokenize();
}