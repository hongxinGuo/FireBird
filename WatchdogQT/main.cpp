#include "WatchdogQT.h"
#include <QtWidgets/QApplication>

#if _MSVC_LANG <= 202002L
#error "本系统使用C++23及以上标准"
#endif

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	WatchdogQT w;
	w.show();
	return a.exec();
}
