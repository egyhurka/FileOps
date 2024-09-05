#ifndef UTILITIES_H
#define UTILITIES_H

#include <filesystem>
#include <string>

bool starts_with(const std::string& str, const char prefix);
bool ends_with(const std::string& str, const char suffix);
std::string SubstrWord(const std::string& str, const int& x);

std::vector<std::string> findArgs(const std::string& str);

// method: dir, file, both
void FilePrinter(const std::vector<std::filesystem::path>& fileList = {}, const std::vector<std::filesystem::path>& dirList = {}, const std::string& method = "dir");
void CommandsHelp();

class Variables {
public:
    std::filesystem::path currentPath = "C:\\";
    bool progIsRunning = true;
};

extern Variables globalVariables;

#endif