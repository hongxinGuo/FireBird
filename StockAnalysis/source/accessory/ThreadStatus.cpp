#include"pch.h"

#include"ThreadStatus.h"

CThreadStatus::CThreadStatus() {
	if (static int siInstance = 0; siInstance++ > 0) {
		TRACE("ϵͳ״ֻ̬��������һ��ʵ��\n");
		gl_systemMessage.PushInformationMessage(_T("ϵͳ״ֻ̬��������һ��ʵ��"));
	}

	m_NumberOfBackGroundWorkingThreads = 0;
	m_NumberOfSavingThread = 0;
	m_NumberOfWebInquiringThread = 0;
}
