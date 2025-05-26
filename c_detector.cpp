#include "c_detector.h"
#include <regex>
#include <algorithm>

CDetector::CDetector() {
    // Initialize C-specific keywords that differ from C++
    cSpecificKeywords = {
        "typedef", "register", "auto", "restrict", "_Bool", "_Complex", "_Imaginary",
        "inline", "volatile"
    };

    // Initialize C-specific headers
    cSpecificHeaders = {
        "stdio.h", "stdlib.h", "string.h", "math.h", "time.h", "ctype.h",
        "stdarg.h", "signal.h", "setjmp.h", "conio.h", "process.h"
    };

    // Initialize C-specific functions
    cSpecificFunctions = {
        "printf", "scanf", "gets", "puts", "getchar", "putchar",
        "malloc", "calloc", "realloc", "free",
        "strcpy", "strcat", "strcmp", "strlen",
        "fopen", "fclose", "fprintf", "fscanf", "fgets", "fputs",
        "getch", "clrscr"
    };
}

bool CDetector::containsCHeader(const std::string& line) {
    for (const auto& header : cSpecificHeaders) {
        // Look for #include <header> or #include "header"
        if (line.find("#include <" + header + ">") != std::string::npos ||
            line.find("#include \"" + header + "\"") != std::string::npos) {
            errors.push_back({0, "C-style header detected: " + header + ". Consider using C++ equivalent."});
            return true;
        }
    }
    return false;
}

bool CDetector::containsCFunction(const std::string& line) {
    for (const auto& func : cSpecificFunctions) {
        // Look for function calls (simple check)
        std::regex funcPattern("\\b" + func + "\\s*\\(");
        if (std::regex_search(line, funcPattern)) {
            errors.push_back({0, "C-style function detected: " + func + ". Consider using C++ equivalent."});
            return true;
        }
    }
    return false;
}

bool CDetector::containsCKeyword(const std::string& line) {
    for (const auto& keyword : cSpecificKeywords) {
        // Look for C-specific keywords
        std::regex keywordPattern("\\b" + keyword + "\\b");
        if (std::regex_search(line, keywordPattern)) {
            errors.push_back({0, "C-style keyword detected: " + keyword + ". Consider using C++ equivalent."});
            return true;
        }
    }
    return false;
}

bool CDetector::containsCCode(const std::string& line, int lineNumber) {
    bool found = false;
    errors.clear();

    // Store the line number for error reporting
    if (containsCHeader(line) || containsCFunction(line) || containsCKeyword(line)) {
        // Update the line numbers in the errors
        for (auto& error : errors) {
            error.first = lineNumber;
        }
        found = true;
    }

    return found;
}

std::vector<std::pair<int, std::string>> CDetector::getErrors() const {
    return errors;
} 