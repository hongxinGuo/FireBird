#pragma once
#include<fmt/core.h>
#include<fcntl.h>

#ifdef _WIN32
#include <windows.h>

struct EnableUTF8 {
	EnableUTF8() {
		SetConsoleOutputCP(CP_UTF8);
		//_setmode(_fileno(stdout), _O_U8TEXT);
	}
};

static EnableUTF8 _enable_utf8;
#define FMT_UTF8
#endif
