#include <Windows.h>

static HANDLE StdOut;
static HANDLE StdErr;

void WriteStdErr(LPCWSTR text) {
	DWORD written;
	WriteConsoleW(StdErr, text, lstrlenW(text), &written, NULL);
}

bool CharToHex(WCHAR wc, BYTE &value) {
	if (L'0' <= wc && wc <= L'9') {
		value = wc - L'0';
	}
	else if (L'A' <= wc && wc <= L'F') {
		value = wc - L'A' + 10;
	}
	else if (L'a' <= wc && wc <= L'f') {
		value = wc - L'a' + 10;
	}
	else {
		return false;
	}
	return true;
}

int Run(int argc, WCHAR **argv) {
	if (argc >= 2) {
		if (lstrcmpW(argv[1], L"x") == 0) {
			HKEY commandProcessorKey = 0;
			LSTATUS result;
			if (0 == (result = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Command Processor", 0, KEY_READ, &commandProcessorKey))) {
				DWORD valueType;
				DWORD dwordValue;
				DWORD cbData = 4;
				if (0 == (result = RegQueryValueExA(commandProcessorKey, "DefaultColor", NULL, &valueType, (LPBYTE)&dwordValue, &cbData))) {
					if (dwordValue == 0) {
						dwordValue = 7;
					}
					SetConsoleTextAttribute(StdOut, (WORD)dwordValue);
					return 0;
				}
			}
			return 2;
		}

		BYTE hi, lo;
		if (CharToHex(argv[1][0], hi) && CharToHex(argv[1][1], lo)) {
			SetConsoleTextAttribute(StdOut, (WORD)((hi << 4) | lo));
			return 0;
		}
	}
	WriteStdErr(
		L"textcolor x ; default\n"
		L"textcolor 07 ; back=0 fore=7\n"
	);
	return 1;
}

void EntryPoint() {
	int argc = 0;
	LPWSTR *argv = CommandLineToArgvW(
		GetCommandLineW(),
		&argc
	);

	StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	StdErr = GetStdHandle(STD_ERROR_HANDLE);

	int retCode = Run(argc, argv);
	ExitProcess(retCode);
}
