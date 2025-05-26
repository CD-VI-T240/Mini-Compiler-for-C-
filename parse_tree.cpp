#include "parse_tree.h"
#include <iostream>
#include <fstream>
#include <sstream>

ParseTreeNode::ParseTreeNode(std::string t, std::string v) : type(t), value(v) {}

void ParseTreeNode::addChild(ParseTreeNode* child) {
    if (child) {
        children.push_back(child);
    }
}

// Helper function to check if current token matches expected type and value
bool match(std::vector<token>& tokens, int pos, tokenType type, const std::string& value = "") {
    if (pos >= tokens.size()) return false;
    if (tokens[pos].type != type) return false;
    if (!value.empty() && tokens[pos].value != value) return false;
    return true;
}

// Parse a factor (number, identifier, or parenthesized expression)
ParseTreeNode* parseFactor(std::vector<token>& tokens, int& pos) {
    if (pos >= tokens.size()) {
        std::cout << "End of tokens while parsing factor" << std::endl;
        return nullptr;
    }
    
    ParseTreeNode* node = nullptr;
    token current = tokens[pos];
    
    if (current.type == number) {
        node = new ParseTreeNode("Number", current.value);
        pos++;
    }
    else if (current.type == identifier) {
        node = new ParseTreeNode("Identifier", current.value);
        pos++;
    }
    else if (current.type == separator && current.value == "(") {
        pos++; // consume '('
        node = new ParseTreeNode("Group");
        ParseTreeNode* expr = parseExpression(tokens, pos);
        if (expr) {
            node->addChild(expr);
        }
        if (pos < tokens.size() && tokens[pos].type == separator && tokens[pos].value == ")") {
            pos++; // consume ')'
        }
    }
    
    return node;
}

// Parse a term (factors separated by * or /)
ParseTreeNode* parseTerm(std::vector<token>& tokens, int& pos) {
    ParseTreeNode* left = parseFactor(tokens, pos);
    if (!left) return nullptr;
    
    while (pos < tokens.size() && tokens[pos].type == operaTor && 
           (tokens[pos].value == "*" || tokens[pos].value == "/")) {
        std::string op = tokens[pos].value;
        pos++; // consume operator
        
        ParseTreeNode* right = parseFactor(tokens, pos);
        if (!right) break;
        
        ParseTreeNode* newNode = new ParseTreeNode("Operation", op);
        newNode->addChild(left);
        newNode->addChild(right);
        left = newNode;
    }
    
    return left;
}

// Parse an expression (terms separated by + or -)
ParseTreeNode* parseExpression(std::vector<token>& tokens, int& pos) {
    ParseTreeNode* left = parseTerm(tokens, pos);
    if (!left) return nullptr;
    
    while (pos < tokens.size() && tokens[pos].type == operaTor && 
           (tokens[pos].value == "+" || tokens[pos].value == "-")) {
        std::string op = tokens[pos].value;
        pos++; // consume operator
        
        ParseTreeNode* right = parseTerm(tokens, pos);
        if (!right) break;
        
        ParseTreeNode* newNode = new ParseTreeNode("Operation", op);
        newNode->addChild(left);
        newNode->addChild(right);
        left = newNode;
    }
    
    return left;
}

// Parse a statement
ParseTreeNode* parseStatement(std::vector<token>& tokens, int& pos) {
    if (pos >= tokens.size()) {
        std::cout << "End of tokens while parsing statement" << std::endl;
        return nullptr;
    }
    
    ParseTreeNode* node = nullptr;
    token current = tokens[pos];
    
    // Variable declaration
    if (current.type == keyword && (current.value == "int" || current.value == "float")) {
        node = new ParseTreeNode("Declaration", current.value);
        pos++; // consume type
        
        if (pos < tokens.size() && tokens[pos].type == identifier) {
            node->addChild(new ParseTreeNode("Identifier", tokens[pos].value));
            pos++; // consume identifier
            
            if (pos < tokens.size() && tokens[pos].type == operaTor && tokens[pos].value == "=") {
                pos++; // consume '='
                ParseTreeNode* expr = parseExpression(tokens, pos);
                if (expr) {
                    node->addChild(expr);
                }
            }
            
            if (pos < tokens.size() && tokens[pos].type == separator && tokens[pos].value == ";") {
                pos++; // consume ';'
            }
        }
    }
    
    return node;
}

// Parse the entire program
ParseTreeNode* parseProgram(std::vector<token>& tokens, int& pos) {
    ParseTreeNode* root = new ParseTreeNode("Program");
    
    while (pos < tokens.size()) {
        ParseTreeNode* stmt = parseStatement(tokens, pos);
        if (stmt) {
            root->addChild(stmt);
        } else {
            pos++; // Skip problematic token
        }
    }
    
    return root;
}

void printParseTree(ParseTreeNode* node, std::string prefix) {
    if (!node) return;
    
    std::cout << prefix << "└─ " << node->type;
    if (!node->value.empty()) {
        std::cout << " (" << node->value << ")";
    }
    std::cout << std::endl;
    
    for (size_t i = 0; i < node->children.size(); i++) {
        bool isLast = (i == node->children.size() - 1);
        std::string newPrefix = prefix + (isLast ? "   " : "│  ");
        printParseTree(node->children[i], newPrefix);
    }
} 