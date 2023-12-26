#include"pch.h"

#include"ThreadStatus.h"

CThreadStatus::CThreadStatus() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	if (static int siInstance = 0; siInstance++ > 0) {
		TRACE("系统状态只允许生成一个实例\n");
		gl_systemMessage.PushInformationMessage(_T("系统状态只允许生成一个实例"));
	}

	m_NumberOfBackGroundWorkingThreads = 0;
	m_NumberOfWebInquiringThread = 0;
}

[[nodiscard]] bool CThreadStatus::IsSavingThreadRunning() noexcept {
	bool bSavingChinaMarketDB = true;
	bool bSavingWorldMarketDB = true;
	if (gl_UpdateChinaMarketDB.try_acquire()) {
		bSavingChinaMarketDB = false;
		gl_UpdateChinaMarketDB.release();
	}
	if (gl_UpdateWorldMarketDB.try_acquire()) {
		bSavingWorldMarketDB = false;
		gl_UpdateWorldMarketDB.release();
	}
	return bSavingChinaMarketDB || bSavingWorldMarketDB;
}
