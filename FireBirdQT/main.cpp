#include "FireBirdQT.h"
#include <QtWidgets/QApplication>

#if _MSVC_LANG <= 201703
#error "本系统使用C++20及以上标准"
#endif

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	FireBirdQT w;
	w.show();
	return a.exec();
}
