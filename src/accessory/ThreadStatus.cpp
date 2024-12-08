/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//Note ���߳�Э��ʹ��״̬
//
// thread_pool_executor: TaskDistributeAndCalculateRTData, CVirtualDataSource::RunWorkingThread, ���ˡ����ס���Ѷʵʱ���ݵĲ��н�����
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
		TRACE("ϵͳ״ֻ̬��������һ��ʵ��\n");
		gl_systemMessage.PushInformationMessage(_T("ϵͳ״ֻ̬��������һ��ʵ��"));
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
