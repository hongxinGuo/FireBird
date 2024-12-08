/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//Note 各线程协程使用状态
//
// thread_pool_executor: TaskDistributeAndCalculateRTData, CVirtualDataSource::RunWorkingThread, 新浪、网易、腾讯实时数据的并行解析。
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"
#include"SystemMessage.h"
import FireBird.System.Configuration;
module FireBird.System.ThreadStatus;


CThreadStatus::CThreadStatus() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	if (static int siInstance = 0; siInstance++ > 0) {
		TRACE("系统状态只允许生成一个实例\n");
		gl_systemMessage.PushInformationMessage(_T("系统状态只允许生成一个实例"));
	}

	m_NumberOfBackGroundWorkingThreads = 0;
	m_NumberOfWebInquiringThread = 0;
}

bool CThreadStatus::IsSavingWorldMarketThreadRunning() noexcept {
	if (gl_UpdateWorldMarketDB.try_acquire()) {
		gl_UpdateWorldMarketDB.release();
		return false;
	}
	return true;
}

bool CThreadStatus::IsSavingChinaMarketThreadRunning() noexcept {
	if (gl_UpdateChinaMarketDB.try_acquire()) {
		gl_UpdateChinaMarketDB.release();
		return false;
	}
	return true;
}

bool CThreadStatus::IsSavingThreadRunning() noexcept {
	return IsSavingChinaMarketThreadRunning() || IsSavingWorldMarketThreadRunning();
}
