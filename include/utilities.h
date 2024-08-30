#ifndef UTILITIES_H
#define UTILITIES_H

#include <filesystem>
#include <string>


bool starts_with(const std::string& str, const char prefix);
bool ends_with(const std::string& str, const char suffix);
std::string SubstrWord(const std::string& str, const int& x);

// method: dir, file, both
void FilePrinter(const std::filesystem::path dirPath, const std::vector<std::filesystem::path>& fileList = {}, const std::vector<std::filesystem::path>& dirList = {}, std::string method = "dir");
void CommandsHelp();


class Variables {
public:
    std::filesystem::path currentPath = "C:\\";
    bool progIsRunning = true;
};

extern Variables globalVariables;

#endif