#include <windows.h>
#include <tlhelp32.h>

#include <string>
#include <vector>

class Memory {
public:
	std::vector<std::pair<std::wstring, DWORD>> GetAllProcess() {
		std::vector<std::pair<std::wstring, DWORD>> processes;

		::PROCESSENTRY32 entry = {};
		entry.dwSize = sizeof(::PROCESSENTRY32);

		HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (processSnapshot == INVALID_HANDLE_VALUE)
			return processes;

		while (::Process32Next(processSnapshot, &entry)) {
			processes.emplace_back(entry.szExeFile, entry.th32ProcessID);
		}

		CloseHandle(processSnapshot);

		return processes;
	}

	DWORD GetProcessID(std::wstring& processName) {
		std::wstring pName = processName;
		if (!processName.ends_with(L".exe"))
			pName = processName + L".exe";

		std::vector<std::pair<std::wstring, DWORD>> processes = GetAllProcess();

		for (const auto& [name, processID] : processes) {
			if (name == pName)
				return processID;
		}
		
		return 0;
	}
};