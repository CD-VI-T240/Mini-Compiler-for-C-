#ifndef C_DETECTOR_H
#define C_DETECTOR_H

#include <string>
#include <vector>
#include <unordered_set>

class CDetector {
public:
    CDetector();
    bool containsCCode(const std::string& line, int lineNumber);
    std::vector<std::pair<int, std::string>> getErrors() const;

private:
    std::unordered_set<std::string> cSpecificKeywords;
    std::unordered_set<std::string> cSpecificHeaders;
    std::unordered_set<std::string> cSpecificFunctions;
    std::vector<std::pair<int, std::string>> errors;
    
    bool containsCHeader(const std::string& line);
    bool containsCFunction(const std::string& line);
    bool containsCKeyword(const std::string& line);
};

#endif // C_DETECTOR_H 