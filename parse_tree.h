#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <string>
#include <vector>
#include "lexical.h"

class ParseTreeNode {
public:
    std::string type;        // Type of the node (e.g., "Statement", "Expression", etc.)
    std::string value;       // Actual value/token
    std::vector<ParseTreeNode*> children;
    
    ParseTreeNode(std::string t, std::string v = "");
    void addChild(ParseTreeNode* child);
};

ParseTreeNode* parseProgram(std::vector<token>& tokens, int& pos);
ParseTreeNode* parseStatement(std::vector<token>& tokens, int& pos);
ParseTreeNode* parseExpression(std::vector<token>& tokens, int& pos);
ParseTreeNode* parseTerm(std::vector<token>& tokens, int& pos);
ParseTreeNode* parseFactor(std::vector<token>& tokens, int& pos);
void printParseTree(ParseTreeNode* node, std::string prefix = "");

#endif // PARSE_TREE_H 