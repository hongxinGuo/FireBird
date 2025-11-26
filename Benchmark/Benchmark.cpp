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

BENCHMARK_MAIN();
