#include"pch.h"

#include<benchmark/benchmark.h>

//#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#ifdef _ASAN
#pragma comment(lib, "asan_d/benchmark.lib")
#pragma comment(lib, "asan_d/benchmark_main.lib")
#else
#pragma comment(lib, "d/benchmark.lib")
#pragma comment(lib, "d/benchmark_main.lib")
#endif
#else
#ifdef _ASAN
#pragma comment(lib, "asan_r/benchmark.lib")
#pragma comment(lib, "asan_r/benchmark_main.lib") 
#else
#pragma comment(lib, "r/benchmark.lib")
#pragma comment(lib, "r/benchmark_main.lib")
#endif
#endif

#ifdef _DEBUG
#pragma comment(lib, "/D/concurrencpp.lib")
#pragma comment(lib, "/concurrencpp/lib/D/concurrencpp.lib")
#else
#pragma comment(lib, "/R/concurrencpp.lib")
#endif

BENCHMARK_MAIN();
