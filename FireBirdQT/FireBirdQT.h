#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FireBirdQT.h"

class FireBirdQT : public QMainWindow {
	Q_OBJECT

public:
	FireBirdQT(QWidget* parent = nullptr);
	~FireBirdQT() override;

protected:
	//void paintEvent(QPaintEvent* event) override;

private slots:
	void doChangeTabTitle(QString title);

private:
	Ui::FireBirdQTClass ui;
};
