#pragma once

#include <QMainWindow>
#include "ui_PropertyWidget.h"

class PropertyWidget : public QMainWindow {
	Q_OBJECT

public:
	PropertyWidget(QWidget* parent = nullptr);
	~PropertyWidget() override;

private:
	Ui::PropertyWidgetClass ui;
};
