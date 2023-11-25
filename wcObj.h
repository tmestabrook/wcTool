#ifndef WCTOOL_WCOBJ_H
#define WCTOOL_WCOBJ_H

#include <iostream>
#include <stack>
#include <fstream>
#include <map>
#include <any>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <chrono>

using CommandFunction = std::function<std::any()>;

class wcObj {
public:
    wcObj(int argc, char* argv[]);
    wcObj(const wcObj& wcCopy);
    ~wcObj();

    std::any getByteCount();
    std::any getLineCount();
    std::any getWordCount();
    std::any getCharCount();

    void executeCommands();
    void countStats();
    void printRuntimeStats();
private:
    std::unique_ptr<std::chrono::time_point<std::chrono::high_resolution_clock>[]> starts =
            std::make_unique<std::chrono::time_point<std::chrono::high_resolution_clock>[]>(2);
    std::unique_ptr<std::chrono::time_point<std::chrono::high_resolution_clock>[]> ends =
            std::make_unique<std::chrono::time_point<std::chrono::high_resolution_clock>[]>(2);
    std::ifstream inputFile;
    std::stack<std::string> commandStack;
    std::map<std::string, CommandFunction> commandMap;
    long long byteCount, charCount;
    int lineCount, wordCount;

    bool parseArgV(const int& argc, char** argv);
    void filenameInput();
};


#endif //WCTOOL_WCOBJ_H
