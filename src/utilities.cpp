#include "../include/utilities.h"

#include <iostream>
#include <map>
#include <functional>

Variables globalVariables;

bool starts_with(const std::string& str, const char prefix) {
    return !str.empty() && str[0] == prefix;
}

bool ends_with(const std::string& str, const char suffix) {
    return !str.empty() && str[str.length() - 1] == suffix;
}

std::string SubstrWord(const std::string& str, const int& x) {
    return str.substr(x);
}

void FilePrinter(const std::filesystem::path dirPath, const std::vector<std::filesystem::path>& fileList, const std::vector<std::filesystem::path>& dirList, std::string method) {
    std::uintmax_t fullSize = 0;

    if (dirList.size() > 0)
        std::wcout << L"\033[33m" << "DIRECTORIES --------------------------------" << L"\033[0m" << std::endl;
    for (const auto& entry : dirList) {
        std::cout << "<DIR> " << "\t\t" << entry.filename().string() << std::endl;
    }

    if (fileList.size() > 0)
        std::wcout << L"\033[33m" << "FILES --------------------------------------" << L"\033[0m" << std::endl;
    for (const auto& entry : fileList) {
        std::cout << "<FILE> " << "\t\t" << entry.filename().string() << std::endl;
        try {
            fullSize += std::filesystem::file_size(entry);
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error reading file size: " << e.what() << std::endl;
        }
    }

    std::map<std::string, std::function<void()>> methodMap = {
            { "file", [&]() {
                double sizeInGB = static_cast<double>(fullSize) / (1024 * 1024 * 1024);
                std::cout << std::fixed << std::setprecision(2);
                std::cout << "\t\t" << fileList.size() << " FILE(s) " << fullSize << " byte | " << sizeInGB << " Gb" << std::endl;
            } },
            { "dir", [&]() { std::cout << "\t\t" << dirList.size() << "DIR(S)" << std::endl; } },
            { "both", [&]() {
                methodMap["file"]();
                methodMap["dir"]();
            } }
    };

    if (methodMap.find(method) != methodMap.end()) {
        methodMap[method]();
    }
    else {
        std::cerr << "Unknown method: " << method << std::endl;
    }
}

void CommandsHelp() {
    std::vector<std::pair<std::string, std::string>> cmdDscp;
    cmdDscp.emplace_back("clear, c", "Clears the screen.");
    cmdDscp.emplace_back("exit, ex, close", "Exits the program.");
    cmdDscp.emplace_back("cd", "Changes the current directory. \n  Args:\t'..' - current path parent.\n\t'./' - go to 'C:\\'.\n\t'\"full path\"' - you can write exact path.");
    cmdDscp.emplace_back("dd", "Displays a list of files and subdirectories in a directory.");
    cmdDscp.emplace_back("df", "Displays a list of files and directories in a directory.");

    for (const auto& pair : cmdDscp) {
        std::cout << pair.first << "\t\t- " << pair.second << "\n";
    }
}