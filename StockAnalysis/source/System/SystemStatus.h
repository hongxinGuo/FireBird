#pragma once

using namespace std;
#include<atomic>

class CSystemStatus : public CObject {
public:
	CSystemStatus();
	virtual ~CSystemStatus() {}

	void SetExitingSystem(bool bExit) { m_fExitingSystem = bExit; }
	bool IsExitingSystem() { return m_fExitingSystem; }
	void SetExitingCalculatingRS(bool bExit) { m_fExitingCalculatingRS = bExit; }
	bool IsExitingCalculatingRS() { return m_fExitingCalculatingRS; }
	void SetNormalMode(bool bNormal) { m_fNormalMode = bNormal; }
	bool IsNormalMode() { return m_fNormalMode; }

protected:
	atomic_bool m_fExitingSystem; //  系统退出标识，用于终止其他线程。
	bool m_fExitingCalculatingRS; // 用于通知工作线程退出的信号
	bool m_fNormalMode; // 正常模式标识，默认为假。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。
};

extern CSystemStatus gl_systemStatus;
