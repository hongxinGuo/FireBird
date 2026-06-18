#include "WatchdogQT.h"
#include <QtWidgets/QApplication>

#if _MSVC_LANG <= 201703
#error "本系统使用C++20及以上标准"
#endif

#ifdef _ASAN
#include<stdlib.h>
extern "C" const char* __asan_default_options() { return "detect_leaks=0"; }
#endif

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	WatchdogQT w;
	w.show();
	return a.exec();
}
