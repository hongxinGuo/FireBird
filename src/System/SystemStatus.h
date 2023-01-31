#pragma once
#include "NeteaseDayLineDataSource.h"
#include "SinaRTDataSource.h"

class CSystemStatus final {
public:
	CSystemStatus();
	// 只能有一个实例,不允许赋值。
	CSystemStatus(const CSystemStatus&) = delete;
	CSystemStatus& operator=(const CSystemStatus&) = delete;
	CSystemStatus(const CSystemStatus&&) noexcept = delete;
	CSystemStatus& operator=(const CSystemStatus&&) noexcept = delete;
	~CSystemStatus() = default;

	void SetExitingSystem(const bool bExit) { m_fExitingSystem = bExit; }
	bool IsExitingSystem() { return m_fExitingSystem; }
	void SetExitingCalculatingRS(const bool bExit) { m_fExitingCalculatingRS = bExit; }
	bool IsExitingCalculatingRS() const { return m_fExitingCalculatingRS; }
	void SetWorkingMode(const bool bNormal) { m_fWorkingMode = bNormal; }
	bool IsWorkingMode() const { return m_fWorkingMode; }

	// 当下载新浪或者网易实时数据出现问题时，系统的其他网络活动应该让步。
	static bool IsWebBusy() { return gl_pSinaRTDataSource->IsWebError() || gl_pNeteaseDayLineDataSource->IsWebError(); }

protected:
	std::atomic_bool m_fExitingSystem; //  系统退出标识，用于终止其他线程。
	bool m_fExitingCalculatingRS; // 用于通知工作线程退出的信号
	bool m_fWorkingMode; // 正常模式标识，默认为假。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。
};

extern CSystemStatus gl_systemStatus;
