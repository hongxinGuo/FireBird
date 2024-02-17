#pragma once

#ifdef _DEBUG
#ifdef _ASAN
#pragma comment(lib, "/D_ASAN/concurrencpp.lib")
#else
#pragma comment(lib, "/D/concurrencpp.lib")
#endif
#else
#ifdef _ASAN
#pragma comment(lib, "/R_ASAN/concurrencpp.lib")
#else
#pragma comment(lib, "/R/concurrencpp.lib")
#endif
#endif

#pragma comment(lib, "/vc/libcrypto64MT.lib")
#pragma comment(lib, "/vc/libssl64MT.lib")
