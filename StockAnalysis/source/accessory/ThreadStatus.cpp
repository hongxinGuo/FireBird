#include"pch.h"

#include"ThreadStatus.h"

CThreadStatus::CThreadStatus() {
	if (static int siInstance = 0; siInstance++ > 0) {
		TRACE("系统状态只允许生成一个实例\n");
		gl_systemMessage.PushInformationMessage(_T("系统状态只允许生成一个实例"));
	}

	m_NumberOfBackGroundWorkingThreads = 0;
	m_NumberOfSavingThread = 0;
	m_NumberOfWebInquiringThread = 0;
}
