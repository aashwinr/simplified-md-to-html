# Simplified md to html converter

> ## About the Syntax
- The syntax for this version of this simplified markdown is slightly different from the original.
- It is trivial to modify the code in order to support the original syntax, I just think this is more clear.

> ## Converter
- The converter just provides a basic html file with no styles.
- To style the quotes target the `quote` class

> ## Syntax

#### Top Level Elements
| Name   | Syntax | Info                                                                 |
|--------|--------|----------------------------------------------------------------------|
| Heading | `#`    | Can be reapted upto 6 times                                          |
| Lists  | `-`    | The number of times it is repeated shows the depth of the list       |
| Quotes | `>`     | The number of times it is repeated shows the depth of the blockquote |

#### Stylistic Elements
| Name            | Syntax                     |
|-----------------|----------------------------|
| Bold/Strong     | `*` **`bold element here`** `*`     |
| Italics/Emphasis | `/` *`italics element here`* `/` |
| Strikethrough   | `~` ~~`ㅤstrikethrough element here`~~ `~`|
| Code blocks     | `` ` `` `code element here` `` ` ``   |

#### Miscellaneous
| Name               | Syntax              |
|--------------------|---------------------|
| Newline/Line Break | Leave an empty line |

> ## Same as the old one but slightly more manageable
- Simplified Lexer/Tokenizer
  - The previous did too much and did some work that was supposed to be done by the parser.
  - Parsing regular text was unnecessarily complicated in the previous version.

> ## Usage
After compiling, run the following command to convert the source file.
- `./simplemdconverter <source_file> <converted_html_file>`

> ## Compilation Instructions
- Use the provided `CMakeLists.txt` file to generate a makefile.
