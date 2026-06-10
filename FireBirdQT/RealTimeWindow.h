#pragma once

#include "ui_RealTimeWindow.h"

class RealTimeWindow : public QMainWindow {
	Q_OBJECT

public:
	RealTimeWindow(QWidget* parent = nullptr);
	~RealTimeWindow() override = default;

private:
	Ui::RealTimeWindowClass ui;
};
