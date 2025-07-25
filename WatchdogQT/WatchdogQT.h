#pragma once

#include <QMessageBox>
#include <QtWidgets/QMainWindow>
#include "ui_WatchdogQT.h"

using std::array;

extern std::chrono::sys_seconds gl_tpNow; // 协调世界时（Coordinated Universal Time）

QT_BEGIN_NAMESPACE

namespace Ui {
	class WatchdogQTClass;
};

QT_END_NAMESPACE

class WatchdogQT : public QMainWindow {
	Q_OBJECT

public:
	WatchdogQT(QWidget* parent = nullptr);
	~WatchdogQT() override;

protected:
	bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;

private:
	void BuildUI();

private slots:
	void Update() const;
	void UpdatePer10Second();
	void onActionAboutTriggered();

private:
	Ui::WatchdogQTClass ui;

private:
	QLabel* labTime;
	QLabel* labStatus;

private:
	std::list<std::string> m_listOutput;
};
