#include "lexical.h"
#include <cctype>
#include <iostream>
#include <regex>

using namespace std;

// Function to convert token type to string for display
string tokenToString(tokenType type) {
    switch(type) {
        case keyword: return "Keyword   ";
        case identifier: return "Identifier";
        case number: return "Number    ";
        case operaTor: return "Operator  ";
        case separator: return "Separator ";
        case string_literal: return "String    ";
        case comment: return "Comment   ";
        default: return "Unknown   ";
    }
}

// Check if a string is a C++ keyword
bool isKeyword(const string& str) {
    static const unordered_set<string> keywords = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
        "class", "namespace", "try", "catch", "throw", "using", "bool", "true",
        "false", "inline", "virtual", "explicit", "export", "friend", "mutable",
        "private", "protected", "public", "template", "typename", "this", "operator",
        "new", "delete"
    };
    return keywords.find(str) != keywords.end();
}

// Function to tokenize the input code
vector<token> tokenize(const string& code) {
    vector<token> tokens;
    string current;
    int line = 1;
    int col = 1;
    
    for (size_t i = 0; i < code.length(); i++) {
        char c = code[i];
        
        // Track line numbers
        if (c == '\n') {
            line++;
            col = 1;
            continue;
        }
        
        // Skip whitespace
        if (isspace(c)) {
            col++;
            continue;
        }
        
        // Handle string literals
        if (c == '"') {
            string str;
            i++;
            col++;
            while (i < code.length() && code[i] != '"') {
                str += code[i];
                i++;
                col++;
            }
            if (i < code.length()) {
                tokens.push_back({string_literal, str, line, col});
            }
            col++;
            continue;
        }
        
        // Handle comments
        if (c == '/' && i + 1 < code.length()) {
            if (code[i + 1] == '/') {
                string comment;
                i += 2;
                col += 2;
                while (i < code.length() && code[i] != '\n') {
                    comment += code[i];
                    i++;
                    col++;
                }
                tokens.push_back({tokenType::comment, comment, line, col});
                continue;
            }
            else if (code[i + 1] == '*') {
                string comment;
                i += 2;
                col += 2;
                while (i + 1 < code.length() && !(code[i] == '*' && code[i + 1] == '/')) {
                    if (code[i] == '\n') {
                        line++;
                        col = 1;
                    } else {
                        comment += code[i];
                        col++;
                    }
                    i++;
                }
                i += 2;
                col += 2;
                tokens.push_back({tokenType::comment, comment, line, col});
                continue;
            }
        }
        
        // Handle numbers
        if (isdigit(c)) {
            string num;
            while (i < code.length() && (isdigit(code[i]) || code[i] == '.')) {
                num += code[i];
                i++;
                col++;
            }
            i--;
            tokens.push_back({number, num, line, col});
            continue;
        }
        
        // Handle operators
        if (string("+-*/%=<>!&|^~").find(c) != string::npos) {
            string op(1, c);
            if (i + 1 < code.length()) {
                char next = code[i + 1];
                if ((c == '+' && next == '+') || (c == '-' && next == '-') ||
                    (c == '=' && next == '=') || (c == '!' && next == '=') ||
                    (c == '<' && next == '=') || (c == '>' && next == '=') ||
                    (c == '&' && next == '&') || (c == '|' && next == '|')) {
                    op += next;
                    i++;
                    col++;
                }
            }
            tokens.push_back({operaTor, op, line, col});
            col++;
            continue;
        }
        
        // Handle separators
        if (string("(){}[];,").find(c) != string::npos) {
            tokens.push_back({separator, string(1, c), line, col});
            col++;
            continue;
        }
        
        // Handle identifiers and keywords
        if (isalpha(c) || c == '_') {
            string identifier;
            while (i < code.length() && (isalnum(code[i]) || code[i] == '_')) {
                identifier += code[i];
                i++;
                col++;
            }
            i--;
            
            // Check if it's a keyword
            if (isKeyword(identifier)) {
                tokens.push_back({keyword, identifier, line, col});
            } else {
                tokens.push_back({tokenType::identifier, identifier, line, col});
            }
            continue;
        }
        
        col++;
    }
    
    return tokens;
}


