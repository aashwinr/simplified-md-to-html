#include "Tokenizer.h"
#include <string>

int main() {
    std::string contents = "# Hello this is a heading\n- This is a *stylized* list\n";
    Tokenizer::Tokenizer tokenizer(contents);
    auto tokens = tokenizer.tokenize();
}