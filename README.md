# Simplified md to html converter

> ## About the Syntax
- The syntax for this markup this version of markup language is slightly different from the original.
- The current version is built in a way that allows for trivial conversion to the original md syntax specification.

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
| Bold/Strong     | **`* bold element here *`**     |
| Italics/Emphasis | *`/ italics element here /`* |
| Strikethrough   | ~~`~ㅤstrikethrough element here~`~~  |
| Code blocks     | `` `code element here` ``   |

#### Miscellaneous
| Name               | Syntax              |
|--------------------|---------------------|
| Newline/Line Break | Leave an empty line |

> ## Same as the old one but slightly more manageable
- Simplified Lexer/Tokenizer
  - The previous did too much and did some work that was supposed to be done by the parser.
  - Parsing regular text was unnecessarily complicated in the previous version.

> ## Compilation Instructions
- Use the provided `CMakeLists.txt` file to generate an executible.
