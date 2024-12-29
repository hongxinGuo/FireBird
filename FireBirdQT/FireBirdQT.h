#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FireBirdQT.h"

class FireBirdQT : public QMainWindow
{
    Q_OBJECT

public:
    FireBirdQT(QWidget *parent = nullptr);
    ~FireBirdQT();

private:
    Ui::FireBirdQTClass ui;
};
