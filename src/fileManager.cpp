#include <iostream>

#include "../include/fileManager.h"
#include "../include/utilities.h"

namespace fs = std::filesystem;

void FileManager::changeDirectory(const std::string& arg) {
    std::string formatted;
    if (arg.length() > 2) formatted = arg.substr(3);
    else return;

    if (starts_with(formatted, '"') && ends_with(formatted, '"')) {
        std::string path = formatted.substr(formatted.find_first_not_of('\"'), formatted.find_last_not_of('\"'));
        if (std::filesystem::exists(path)) {
            globalVariables.currentPath = path;
        }
        else {
            std::cout << "An unknown error occurred while changing directory." << std::endl;
        }
    }
    else if (formatted == ".." || formatted == ".") {
        globalVariables.currentPath = globalVariables.currentPath.parent_path();
    }
    else if (formatted == "./") {
        globalVariables.currentPath = "C:\\";
    }
    else if (arg.find("cd") != std::string::npos) {
        std::filesystem::path p = globalVariables.currentPath / formatted;
        if (std::filesystem::exists(p)) {
            globalVariables.currentPath = p;
        }
        else {
            std::cout << "An unknown error occurred while changing directory." << std::endl;
        }
    }
    else {
        if (arg != "cd")
            std::cout << arg << "is not recognizable. " << std::endl;
    }
}

std::vector<std::filesystem::path> FileManager::listDirectory(const std::filesystem::path& path, const bool& directory) {
    std::vector<std::filesystem::path> list;
    if (fs::is_directory(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (directory && entry.is_directory()) {
                list.push_back(entry.path());
            }
            else if (!directory && entry.is_regular_file()) {
                list.push_back(entry.path());
            }
        }
    }
    else {
        std::cout << "The specified path is not a directory." << std::endl;
    }

    return list;
}


void FileManager::copyFile(const std::string& path, const std::string& destination) {
    try {
        if (!fs::exists(path)) {
            std::cout << "Error: The specified path does not exist." << std::endl;
            return;
        }

        if (!fs::is_directory(destination)) {
            std::cout << "Error: The target path is not a directory." << std::endl;
            return;
        }

        fs::path destPath = fs::path(destination) / fs::path(path).filename();

        if (fs::is_regular_file(path)) {
            fs::copy_file(path, destPath, fs::copy_options::overwrite_existing);
        }

        else if (fs::is_directory(path)) {
            fs::copy(path, destPath, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        }
        else {
            std::cout << "Error: The specified path is not a file or directory." << std::endl;
        }
    }
    catch (const fs::filesystem_error& e) {
        std::wcerr << L"Filesystem error: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "An unknown error occurred while copying the file or directory." << std::endl;
    }
}

void FileManager::renameFile(const std::string& name, const std::string& newName) {
    fs::path oldPath = globalVariables.currentPath / name;
    fs::path newPath = globalVariables.currentPath / newName;

    if (!fs::exists(oldPath)) {
        std::cout << "Error: The specified file or directory does not exist: " << oldPath << std::endl;
        return;
    }

    if (fs::exists(newPath)) {
        std::cout << "Error: A file or directory with the new name already exists: " << newPath << std::endl;
        return;
    }

    try {
        fs::rename(oldPath, newPath);
    }
    catch (const fs::filesystem_error& e) {
        std::cout << "Error renaming file: " << e.what() << std::endl;
    }
}

void FileManager::deleteFile(const std::string& path) {
    fs::path p = globalVariables.currentPath / path;
    if (fs::exists(p)) {
        fs::remove(p);
    }
    else {
        std::cout << "Error: The specified path does not exist." << std::endl;
    }
};

void FileManager::createDirectory(const std::string& name) {
    if (name.empty()) {
        std::cout << "Error: Directory name cannot be empty." << std::endl;
        return;
    }

    fs::path newDirPath = globalVariables.currentPath / name;

    if (fs::exists(newDirPath)) {
        std::cout << "Error: The specified directory already exists." << std::endl;
        return;
    }

    if (fs::is_directory(globalVariables.currentPath)) {
        try {
            fs::create_directory(newDirPath);
        }
        catch (const fs::filesystem_error& e) {
            std::wcerr << L"Error creating directory: " << e.what() << std::endl;
        }
    }
    else {
        std::cout << "Error: The specified path does not exist or is not a directory." << std::endl;
    }
}
