#include"pch.h"

#include"ThreadStatus.h"

CThreadStatus::CThreadStatus() {
	static int siInstance = 0;
	if (siInstance++ > 0) {
		TRACE("ϵͳ״ֻ̬��������һ��ʵ��\n");
		gl_systemMessage.PushInformationMessage(_T("ϵͳ״ֻ̬��������һ��ʵ��"));
	}

	m_NumberOfBackGroundWorkingThreads = 0;
	m_NumberOfSavingThread = 0;
	m_NumberOfWebInquiringThread = 0;
}
