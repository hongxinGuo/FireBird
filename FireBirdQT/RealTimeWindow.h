#pragma once

#include <QMainWindow>
#include "ui_RealTimeWindow.h"

class RealTimeWindow : public QMainWindow {
	Q_OBJECT

public:
	RealTimeWindow(QWidget* parent = nullptr);
	~RealTimeWindow() override;

private:
	Ui::RealTimeWindowClass ui;
};
