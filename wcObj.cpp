#include "wcObj.h"


wcObj::wcObj(int argc, char **argv) : byteCount(0), charCount(0), lineCount(0), wordCount(0) {
    starts[0] = std::chrono::high_resolution_clock::now();
    starts[1] = starts[0];
    ends[0] = starts[0];
    ends[1] = starts[0];
    if (!parseArgV(argc, argv)) {
        ends[0] = std::chrono::high_resolution_clock::now();
        filenameInput();
        starts[1] = std::chrono::high_resolution_clock::now();
    }
    commandMap["-c"] = [this]() { return this->getByteCount(); };
    commandMap["-l"] = [this]() { return this->getLineCount(); };
    commandMap["-m"] = [this]() { return this->getCharCount(); };
    commandMap["-w"] = [this]() { return this->getWordCount(); };
}

wcObj::wcObj(const wcObj &wcCopy) {}

wcObj::~wcObj() {
    inputFile.close();
}

void wcObj::executeCommands() {
    if (commandStack.empty()) {
        getLineCount();
        getWordCount();
        getByteCount();
    }
    while (!commandStack.empty()) {
        std::string currCommand = commandStack.top();
        commandStack.pop();
        commandMap[currCommand]();
    }
    if (ends[0] == starts[0]) {
        ends[0] = std::chrono::high_resolution_clock::now();
    } else {
        ends[1] = std::chrono::high_resolution_clock::now();
    }
}

std::any wcObj::getByteCount() {
    std::cout << std::left << std::setw(20) << "Byte count:" << std::setw(10)
              << byteCount << std::endl;
    return byteCount;
}

std::any wcObj::getLineCount() {
    std::cout << std::left << std::setw(20) << "Line count:" << std::setw(10) << lineCount << std::endl;
    return lineCount;
}

std::any wcObj::getWordCount() {
    std::cout<< std::left << std::setw(20) << "Word count:" << std::setw(10)
              << wordCount << std::endl;
    return wordCount;
}

std::any wcObj::getCharCount() {
    std::cout<< std::left << std::setw(20) << "Char count:" << std::setw(10)
             << charCount << std::endl;
    return charCount;

}

void wcObj::countStats() {
    byteCount = inputFile.tellg();
    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);
    bool inWord = false;
    std::string bytes((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    for (unsigned char c: bytes) {
        if ((c & 0xC0) != 0x80) {
            charCount++;
        }
        if (c == '\n') {
            lineCount++;
        }
        if (std::isspace(c)) {
            if (inWord) {
                wordCount++;
                inWord = false;
            }
        } else {
            inWord = true;
        }
    }
}

void wcObj::printRuntimeStats() {
    auto t1 = std::chrono::duration_cast<std::chrono::microseconds>(ends[0] - starts[0] + ends[1] - starts[1]);
    std::cout << std::left << std::setw(20) << "Program runtime:" << std::setprecision(8) << t1.count() / 1000.0 << "ms" << std::endl;
}

bool wcObj::parseArgV(const int& argc, char** argv) {
    const char *ext = ".txt";
    size_t xLen = strlen(ext);

    for (size_t i = 1; i < argc; i++) {
        size_t sLen = strlen(argv[i]);
        if (!strcmp(argv[i] + sLen - xLen, ext)) {
            inputFile.open(argv[i], std::ios::binary | std::ios::ate);
            return true;
        } else {
            commandStack.emplace(argv[i]);
        }
    }
    return false;
}

void wcObj::filenameInput() {
    std::string filename;
    const std::string ext = ".txt";
    int extLen = ext.length();
    while (filename.length() <= extLen || (filename.length() > extLen && filename.compare(filename.length() - extLen, extLen, ext))) {
        std::cout << "Please enter a file path to a .txt file and press enter or press enter with no input to exit the program:\n";
        getline(std::cin, filename);
        if (filename.empty()) {
            exit(0);
        }
    }

    if (!filename.empty()) {
        inputFile.open(filename, std::ios::binary | std::ios::ate);
        if (!inputFile.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            exit(1);
        }
    }
}