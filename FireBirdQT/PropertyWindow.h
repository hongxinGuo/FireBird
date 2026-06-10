#pragma once

#include "ui_PropertyWindow.h"

class PropertyWindow : public QMainWindow {
	Q_OBJECT

public:
	PropertyWindow(QWidget* parent = nullptr);
	~PropertyWindow() override = default;

private:
	Ui::PropertyWindowClass ui;
};
