#include "WatchdogQT.h"
#include <QtWidgets/QApplication>

#if _MSVC_LANG <= 202002L
#error "本系统使用C++23及以上标准"
#endif

// QT版本检查, 6.12.X是最后一个支持Windows10的版本, QT6.13.X及以上版本不再支持Windows10。
#if QT_VERSION >= QT_VERSION_CHECK(6, 13, 0)
#error "适用于Windows10的最后版本为QT6.12.X"
#endif

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	WatchdogQT w;
	w.show();
	return a.exec();
}
