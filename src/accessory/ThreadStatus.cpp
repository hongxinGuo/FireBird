#include "ThreadStatus.h"
#include"pch.h"

#include"ThreadStatus.h"

CThreadStatus::CThreadStatus() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	if (static int siInstance = 0; siInstance++ > 0) {
		TRACE("ϵͳ״ֻ̬��������һ��ʵ��\n");
		gl_systemMessage.PushInformationMessage(_T("ϵͳ״ֻ̬��������һ��ʵ��"));
	}

	m_NumberOfBackGroundWorkingThreads = 0;
	m_NumberOfWebInquiringThread = 0;
}

bool CThreadStatus::IsSavingWorldMarketThreadRunning() noexcept {
	bool bSavingWorldMarketDB = true;
	if (gl_UpdateWorldMarketDB.try_acquire()) {
		bSavingWorldMarketDB = false;
		gl_UpdateWorldMarketDB.release();
	}
	return bSavingWorldMarketDB;
}

bool CThreadStatus::IsSavingChinaMarketThreadRunning() noexcept {
	bool bSavingChinaMarketDB = true;
	if (gl_UpdateChinaMarketDB.try_acquire()) {
		bSavingChinaMarketDB = false;
		gl_UpdateChinaMarketDB.release();
	}

	return bSavingChinaMarketDB;
}
[[nodiscard]] bool CThreadStatus::IsSavingThreadRunning() noexcept {
	return IsSavingChinaMarketThreadRunning() || IsSavingWorldMarketThreadRunning();
}
