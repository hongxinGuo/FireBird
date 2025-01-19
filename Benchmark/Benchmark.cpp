#include"pch.h"

#include<benchmark/benchmark.h>

#ifdef _DEBUG
#pragma comment(lib, "d/benchmark.lib")
#pragma comment(lib, "d/benchmark_main.lib")
//#pragma comment(lib, "/vc/x64/MTd/libcrypto.lib")
//#pragma comment(lib, "/vc/x64/MTd/libssl.lib")
//#pragma comment(lib, "/D/concurrencpp.lib")
#else
#pragma comment(lib, "r/benchmark.lib")
#pragma comment(lib, "r/benchmark_main.lib")
//#pragma comment(lib, "/vc/x64/MT/libcrypto.lib")
//#pragma comment(lib, "/vc/x64/MT/libssl.lib")
//#pragma comment(lib, "/R/concurrencpp.lib")
#endif

BENCHMARK_MAIN();
