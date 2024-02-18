#include"pch.h"

#include<benchmark/benchmark.h>

#ifdef _DEBUG
#ifdef _ASAN
#pragma comment(lib, "asan_d/benchmark.lib")
#pragma comment(lib, "asan_d/benchmark_main.lib")
#else
#pragma comment(lib, "d/benchmark.lib")
#pragma comment(lib, "d/benchmark_main.lib")
#endif
#else
#pragma comment(lib, "r/benchmark.lib")
#pragma comment(lib, "r/benchmark_main.lib")
#endif

BENCHMARK_MAIN();
