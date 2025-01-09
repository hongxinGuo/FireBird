#pragma once

#include <QMainWindow>
#include "ui_PropertyWindow.h"

class PropertyWindow : public QMainWindow {
	Q_OBJECT

public:
	PropertyWindow(QWidget* parent = nullptr);
	~PropertyWindow() override;

private:
	Ui::PropertyWindowClass ui;
};
