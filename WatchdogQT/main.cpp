#include "WatchdogQT.h"
#include <QtWidgets/QApplication>

#if _MSVC_LANG <= 201703
#error "��ϵͳʹ��C++20�����ϱ�׼"
#endif

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	WatchdogQT w;
	w.show();
	return a.exec();
}
