#include "Windows.h"
#include "stdio.h"
#include "str.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")

int main(int argc, char const *argv[]) {
	if (argc == 2 && cmplx::strcmp(argv[1], "install-context")) {

		if (MessageBoxA(0, "Install Pathboi into Contextmenu?", "Pathboi", MB_YESNO | MB_ICONQUESTION) == IDNO) return 0;

		HKEY shell;
		RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Classes\\Directory\\Background\\shell", 0, KEY_ALL_ACCESS, &shell);

		HKEY pathboi;
		RegCreateKeyA(shell, "Pathboi", &pathboi);
		RegSetValueExA(pathboi, "Extended", 0, REG_SZ, (LPBYTE)"", 1);
		RegSetValueExA(pathboi, 0, 0, REG_SZ, (LPBYTE)"Add to Path...", 15);

		HKEY command;
		RegCreateKeyA(pathboi, "command", &command);

		char buf[MAX_PATH];
		GetModuleFileNameA(0, buf, MAX_PATH);

		RegSetValueExA(command, 0, 0, REG_SZ, (LPBYTE)buf, cmplx::strlen(buf) + 1);

		return 0;
	}

	HKEY env;
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "System\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_ALL_ACCESS, &env);

	DWORD size = 4096;
	char* buffer = new char[size];
	DWORD type = REG_SZ;
	RegQueryValueExA(env, "Path", 0, &type, (LPBYTE)buffer, &size);

	Str path(buffer);
	delete[] buffer;

	Str cd;

	if (argc == 2) {
		cd = argv[1];
	} else {
		buffer = new char[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, buffer);

		cd = Str(buffer);
		delete[] buffer;
	}

	if (MessageBoxA(0, str("Do you want to Add \"" + cd + "\" to Path?"), "Pathboi", MB_YESNO | MB_ICONQUESTION) == IDYES) {
		Str newPath = strn(path + cd + ";");
		RegSetValueExA(env, "Path", 0, type, (LPBYTE)newPath.c_str(), newPath.len());
		SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", 0, 1000, 0);
	}

	return 0;
}
