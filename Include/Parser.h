// Created by Ashwin Rohit on 5/2/22.

#ifndef PROPERMDTRANSLATER_PARSER_H
#define PROPERMDTRANSLATER_PARSER_H

#include <vector>
#include "Tokenizer.h"

namespace simpleconv {

    using namespace std;

    enum class MdTokenKind {
        Heading,
        List,
        Quote,
        Text,
        Bold,
        Italics,
        Code,
        NewLine,
    };

    class Parser {

    };
}

#endif //PROPERMDTRANSLATER_PARSER_H
