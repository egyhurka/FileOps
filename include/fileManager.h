#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include <filesystem>

class FileManager {
public:
    void changeDirectory(const std::string& arg);
    std::vector<std::filesystem::path>listDirectory(const std::filesystem::path path, const bool directory);
    void copyFile(const std::wstring& path, const std::wstring& destination) noexcept;
    void renameFile(const std::string& path, const std::string& newName);
    void deleteFile(const std::wstring& path);
    void createDirectory(const std::wstring& path);
};

#endif