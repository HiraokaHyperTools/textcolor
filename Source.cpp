#include <Windows.h>
#include <stdio.h>

int main(int argc, char **argv) {
	if (argc >= 2) {
		int color;
		if (strcmp(argv[1], "x") == 0) {
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
					SetConsoleTextAttribute(
						GetStdHandle(STD_OUTPUT_HANDLE),
						(WORD)dwordValue
					);
					return 0;
				}
			}
			return 2;
		}
		if (sscanf_s(argv[1], "%x", &color) == 1) {
			SetConsoleTextAttribute(
				GetStdHandle(STD_OUTPUT_HANDLE),
				(WORD)color
			);
			return 0;
		}
	}
	fprintf(stderr,
		"textcolor x ; default\n"
		"textcolor 07 ; back=0 fore=7\n"
	);
	return 1;
}
