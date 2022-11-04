#include"pch.h"

#include<benchmark/benchmark.h>

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#pragma comment(lib, "debug/benchmark.lib")
#pragma comment(lib, "debug/benchmark_main.lib")
#else
#pragma comment(lib, "release/benchmark.lib")
#pragma comment(lib, "release/benchmark_main.lib")
#endif

BENCHMARK_MAIN();