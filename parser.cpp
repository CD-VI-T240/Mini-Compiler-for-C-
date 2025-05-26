#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "lexical.h"
#include "parse_tree.h"

using namespace std;

void printUsage() {
    cout << "Usage: parser <input_file> [options]\n"
         << "Options:\n"
         << "  --lexical     Run only lexical analysis\n"
         << "  --parse-tree  Run only parse tree generation\n"
         << "  (no options)  Run all analyses\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    string code;
    bool lexical_only = false;
    bool parse_tree_only = false;

    // Parse command line arguments
    for (int i = 2; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--lexical") {
            lexical_only = true;
        } else if (arg == "--parse-tree") {
            parse_tree_only = true;
        }
    }

    // Read input file
    ifstream inFile(argv[1]);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open input file: " << argv[1] << endl;
        return 1;
    }
    
    stringstream buffer;
    buffer << inFile.rdbuf();
    code = buffer.str();
    inFile.close();

    // Perform lexical analysis first (needed for both modes)
    vector<token> tokens = tokenize(code);

    if (lexical_only) {
        // Only show lexical analysis
        cout << "Tokens found:\n";
        for (const auto& tok : tokens) {
            cout << tokenToString(tok.type) << ": " << tok.value 
                 << " (line " << tok.line << ", col " << tok.col << ")\n";
        }
    }
    else if (parse_tree_only) {
        // Only show parse tree
        int pos = 0;
        ParseTreeNode* root = parseProgram(tokens, pos);
        
        if (root) {
            cout << "Parse Tree:\n";
            printParseTree(root, "");
        } else {
            cout << "Failed to generate parse tree!\n";
        }
    }
    else {
        // Show both analyses
        cout << "Running full analysis...\n\n";
        
        cout << "1. Lexical Analysis:\n";
        cout << "Tokens found:\n";
        for (const auto& tok : tokens) {
            cout << tokenToString(tok.type) << ": " << tok.value 
                 << " (line " << tok.line << ", col " << tok.col << ")\n";
        }
        cout << "\n2. Parse Tree Analysis:\n";
        
        int pos = 0;
        ParseTreeNode* root = parseProgram(tokens, pos);
        
        if (root) {
            cout << "Parse Tree:\n";
            printParseTree(root, "");
        } else {
            cout << "Failed to generate parse tree!\n";
        }
    }

    return 0;
} 