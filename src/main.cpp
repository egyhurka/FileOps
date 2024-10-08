#include <windows.h>
#include <Lmcons.h>

#include <iostream>
#include <functional>
#include <sstream>
#include <map>

#include "../include/fileManager.h"
#include "../include/utilities.h"
#include "memory.cpp"

static char uInput[_MAX_PATH];
std::string windowTitle = "FileOps | v: 0.1";

std::vector<std::string> commands = { "clear", "c", "exit", "ex", "close", "help", "?", "cd", "dd", "df", "ddf", "md", "cp", "ren", "rm" };

void handleCommand(std::string& rawUserCommand, std::vector<std::string> args) {
    FileManager fm;
    std::string foundedCommand;
    std::map<std::string, std::function<void()>> commandMap = {
        { "clear", [&]() { system("cls"); } },
        { "c", [&]() { system("cls"); } },
        { "exit", [&]() { globalVariables.progIsRunning = false; } },
        { "ex", [&]() { globalVariables.progIsRunning = false; } },
        { "close", [&]() { globalVariables.progIsRunning = false; } },
        { "help", [&]() { CommandsHelp(); } },
        { "?", [&]() { CommandsHelp(); } },
        { "cd", [&]() { fm.changeDirectory(rawUserCommand); } },
        { "dd", [&]() { FilePrinter({}, fm.listDirectory(globalVariables.currentPath, true), "dir"); } },
        { "df", [&]() { FilePrinter(fm.listDirectory(globalVariables.currentPath, false), {}, "file"); } },
        { "ddf", [&]() { FilePrinter(fm.listDirectory(globalVariables.currentPath, false), fm.listDirectory(globalVariables.currentPath, true), "both"); } },
        { "md", [&]() { fm.createDirectory(args[0]); } },
        { "cp", [&]() { fm.copyFile(args[0], args[1]); } },
        { "ren", [&]() { fm.renameFile(args[0], args[1]); } },
        { "rm", [&]() { fm.deleteFile(args[0]); } }
    };

    std::string firstWord = rawUserCommand.substr(0, rawUserCommand.find(' '));

    for (const auto& [command, action] : commandMap) {
        if (firstWord == command) {
            foundedCommand = command;
            break;
        }
    }

    if (!foundedCommand.empty()) {
        commandMap[foundedCommand]();
    }
    else {
        std::cout << "Command not found!" << std::endl;
    }
}

void processCommand(std::string& rawUserCommand) {
    bool commandFound = false;
    std::string foundedCommand;

    for (const auto& e : commands) {
        if (rawUserCommand.find(e) != std::string::npos) {
            commandFound = true;
            foundedCommand = e;
            break;
        }
    }

    if (commandFound && !foundedCommand.empty()) {
        handleCommand(rawUserCommand, findArgs(rawUserCommand));
    }
    else {
        std::cout << rawUserCommand << " is not recognizable. " << std::endl;
    }
}


int main() {

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleTitleA(windowTitle.c_str());

    TCHAR winUser[UNLEN + 1];
    DWORD winUserSize = UNLEN + 1;

    std::wstring username = L"guest";

    if (GetUserName((TCHAR*)winUser, &winUserSize)) {
        username = winUser;
    }

	while (globalVariables.progIsRunning) {
        std::wcout << L"\033[32m" << username << L"\033[0m\033[33m ~ \033[0m" << globalVariables.currentPath.c_str() << std::endl;
        std::cout << "-$: ";
        std::cin.getline(uInput, _MAX_PATH);
        std::string rawUserCommand = uInput;
        processCommand(rawUserCommand);
	}

    exit(0);
}