#include "../include/utilities.h"

#include <iostream>
#include <map>
#include <functional>
#include <regex>

Variables globalVariables;

namespace fs = std::filesystem;

bool starts_with(const std::string& str, const char prefix) {
    return !str.empty() && str[0] == prefix;
}

bool ends_with(const std::string& str, const char suffix) {
    return !str.empty() && str[str.length() - 1] == suffix;
}

std::string SubstrWord(const std::string& str, const int& x) {
    return str.substr(x);
}

std::vector<std::string> findArgs(const std::string& str) {
    std::vector<std::string> args;

    std::istringstream iss(str);
    std::string command;
    iss >> command;

    std::string remaining;
    std::getline(iss, remaining);

    std::regex argRegex(R"((\"[^\"]*\")|([^\s]+))");
    auto words_begin = std::sregex_iterator(remaining.begin(), remaining.end(), argRegex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string matchedStr = match.str();

        // Remove surrounding quotes if present
        if (matchedStr.front() == '"' && matchedStr.back() == '"') {
            matchedStr = matchedStr.substr(1, matchedStr.size() - 2);
        }

        args.push_back(matchedStr);
    }

    return args;
}

// method: dir, file, both
void FilePrinter(const std::vector<std::filesystem::path>& fileList, const std::vector<std::filesystem::path>& dirList, const std::string& method) {
    std::uintmax_t fullSize = 0;

    if (!dirList.empty()) {
        std::wcout << L"\033[33mDIRECTORIES --------------------------------\033[0m" << std::endl;
        for (const auto& entry : dirList) {
            std::wcout << L"<DIR> \t\t" << entry.filename().wstring() << std::endl;
        }
    }

    if (!fileList.empty()) {
        std::wcout << L"\033[33mFILES --------------------------------------\033[0m" << std::endl;
        for (const auto& entry : fileList) {
            std::wcout << L"<FILE> \t\t" << entry.filename().wstring() << std::endl;
            try {
                fullSize += fs::file_size(entry);
            }
            catch (const fs::filesystem_error& e) {
                std::cerr << "Error reading file size: " << e.what() << std::endl;
            }
        }
    }

    std::map<std::string, std::function<void()>> methodMap = {
        { "file", [&]() {
            double sizeInGB = static_cast<double>(fullSize) / (1024 * 1024 * 1024);
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "\t\t" << fileList.size() << " FILE(s) " << fullSize << " byte | " << sizeInGB << " GB" << std::endl;
        }},
        { "dir", [&]() { std::cout << "\t\t" << dirList.size() << " DIR(S)" << std::endl; }},
        { "both", [&]() {
            methodMap["file"]();
            methodMap["dir"]();
        }}
    };

    auto it = methodMap.find(method);
    if (it != methodMap.end()) {
        it->second();
    }
    else {
        std::cerr << "Unknown method: " << method << std::endl;
    }
}

void CommandsHelp() {
    std::vector<std::pair<std::string, std::string>> cmdDscp;
    cmdDscp.emplace_back("clear, c", "Clears the screen.");
    cmdDscp.emplace_back("exit, ex, close", "Exits the program.");
    cmdDscp.emplace_back("help, ?", "Displays help information for commands.");
    cmdDscp.emplace_back("cd", "Changes the current directory.\n  Args:\t'..' - move to the parent directory.\n\t'./' - go to 'C:\\'.\n\t'\"full path\"' - specify an exact path.");
    cmdDscp.emplace_back("dd", "Displays a list of subdirectories in the current directory.");
    cmdDscp.emplace_back("df", "Displays a list of files in the current directory.");
    cmdDscp.emplace_back("ddf", "Displays a list of files and subdirectories in the current directory.");
    cmdDscp.emplace_back("md", "Creates a new directory.\n  Args:\t'directory name' - specify the name of the new directory.");
    cmdDscp.emplace_back("cp", "Copies a file or directory to another location.\n  Args:\t'\"full path source\"' to '\"full path destination\"' - specify the source and destination paths.");
    cmdDscp.emplace_back("ren", "Renames a file or directory.\n  Args:\t'old name' to 'new name' - specify the old and new names.");
    cmdDscp.emplace_back("rm", "Removes a file or directory.\n  Args:\t'name' - specify the name of the file or directory to remove.");


    for (const auto& pair : cmdDscp) {
        std::cout << pair.first << "\t\t- " << pair.second << "\n";
    }
}