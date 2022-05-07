//
// Created by Zirconium  on 5/7/22.
//

#ifndef PROPERMDTRANSLATER_CONSTANTS_H
#define PROPERMDTRANSLATER_CONSTANTS_H

#include <string>

namespace simplemdconverter {
    using namespace std;
    namespace constants {
        static const string style = \
        "<style>"
        "html { font-family: sans-serif; }"
        "body { width: 75%; margin: 20px 12.5%; }"
        "</style>";
        static const string html_body_opening = "<html><head><title>Output</title>" + style + "</head><body>";
        static const string html_body_terminating = "</body></html>";
    }
}

#endif //PROPERMDTRANSLATER_CONSTANTS_H
