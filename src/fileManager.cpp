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
            std::wcerr << L"An unknown error occurred while changing directory." << std::endl;
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
            std::wcerr << L"An unknown error occurred while changing directory." << std::endl;
        }
    }
    else {
        if (arg != "cd")
            std::cout << arg << "is not recognizable. " << std::endl;
    }
}

std::vector<std::filesystem::path> FileManager::listDirectory(const std::filesystem::path path, const bool directory) {
	std::vector<std::filesystem::path> directories;
	if (fs::is_directory(path)) {
		for (const auto& entry : fs::directory_iterator(path)) {
			if (directory && !entry.is_regular_file()) {
					directories.push_back(entry.path());
			}
			else {
				if (!entry.is_directory())
					directories.push_back(entry.path());
			}
		}
	}
	else {
		std::wcerr << L"The specified path is not a directory." << std::endl;
	}

	return directories;
}

void FileManager::copyFile(const std::wstring& path, const std::wstring& destination) noexcept {
    try {
        if (!fs::exists(path)) {
            std::wcerr << L"Error: The specified path does not exist." << std::endl;
            return;
        }

        if (!fs::is_directory(destination)) {
            std::wcerr << L"Error: The target path is not a directory." << std::endl;
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
            std::wcerr << L"Error: The specified path is not a file or directory." << std::endl;
        }
    }
    catch (const fs::filesystem_error& e) {
        std::wcerr << L"Filesystem error: " << e.what() << std::endl;
    }
    catch (...) {
        std::wcerr << L"An unknown error occurred while copying the file or directory." << std::endl;
    }
}
void FileManager::renameFile(const std::string& path, const std::string& newName) {
    if (fs::is_directory(path)) {
        fs::path newFilePath = fs::path(path).parent_path() / newName;
        fs::rename(path, newFilePath);
    }
};

void FileManager::deleteFile(const std::wstring& path) {
    if (fs::exists(path)) {
        fs::remove(path);
    }
    else {
        std::wcerr << L"Error: The specified path does not exist." << std::endl;
    }
};

void FileManager::createDirectory(const std::wstring& path) {
    fs::path parentPath = fs::path(path).parent_path();

    if (fs::exists(path)) {
        std::wcerr << L"Error: The specified path is already exists." << std::endl;
        return;
    }

    if (fs::is_directory(parentPath) && fs::exists(parentPath)) {
        fs::create_directory(path);
    }
    else {
        std::wcerr << L"Error: The specified path does not exist or not a directory." << std::endl;
    }
};