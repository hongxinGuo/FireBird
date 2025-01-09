#include "FireBirdQT.h"

#include <QPainter>
#include <QPaintEvent>

FireBirdQT::FireBirdQT(QWidget* parent) : QMainWindow(parent) {
	ui.setupUi(this);

	//this->setWindowState(Qt::WindowMaximized);

	setWindowIcon(QIcon(":/FireBirdQT/Res/FireBird.ico"));
}

FireBirdQT::~FireBirdQT() {
	// 创建状态栏
}

/*
void FireBirdQT::paintEvent(QPaintEvent* event) {
	QPainter painter(this);

	painter.drawPixmap(0, ui.mainToolBar->height() + ui.menuBar->height(), this->width(), this->height() - ui.menuBar->height() - ui.mainToolBar->height() - ui.statusBar->height(),
	                   QPixmap(":/FireBirdQT/Res/Stone.jpg"));
	event->accept();
}
*/

void FireBirdQT::doChangeTabTitle(QString title) {
	//int index = ui.tabWidget->currentIndex();
	//ui.tabWidget->setTabText(index, title);
}
