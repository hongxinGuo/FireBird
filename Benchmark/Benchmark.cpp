#include"pch.h"

#undef max

#include<benchmark/benchmark.h>

#ifdef _DEBUG
#pragma comment(lib, "d/benchmark.lib")
#pragma comment(lib, "d/benchmark_main.lib")
#else
#pragma comment(lib, "r/benchmark.lib")
#pragma comment(lib, "r/benchmark_main.lib")
#endif

#ifdef _ASAN
#include<stdlib.h>
extern "C" const char* __asan_default_options() { return "detect_leaker"; }
#endif

BENCHMARK_MAIN();
