#ifndef LEXICAL_H
#define LEXICAL_H

#include <string>
#include <vector>
#include <unordered_set>

enum tokenType {
    keyword,
    identifier,
    number,
    operaTor,
    separator,
    string_literal,
    comment,
    unknown
};

struct token {
    tokenType type;
    std::string value;
    int line;
    int col;
};

std::string tokenToString(tokenType type);
std::vector<token> tokenize(const std::string& code);

#endif // LEXICAL_H