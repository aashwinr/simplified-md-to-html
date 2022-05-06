#include "Include/Tokenizer.h"
#include "Include/Parser.h"
#include "Include/Generator.h"
#include <string>
#include <fstream>
#include <iostream>

int main() {
    simpleconv::Generator::convert("../hello.md", "../hello.html");
}