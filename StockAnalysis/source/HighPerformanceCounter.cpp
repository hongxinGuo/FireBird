#include "pch.h"
#include "HighPerformanceCounter.h"

long long CHighPerformanceCounter::s_performanceFrequency = 10000000;// Ƶ�ʼ��������߾��ȼ�����ÿ��Ƶ��. Ĭ��Ϊ1ǧ��i710700KF��������Ĭ��ֵ����
bool CHighPerformanceCounter::s_isInitialized = false;

CHighPerformanceCounter::CHighPerformanceCounter() {
	m_start.QuadPart = 0;
	m_end.QuadPart = 0;
	if (!s_isInitialized) {
		// ��ʼ���߾��ȼ�ʱ��Ƶ��
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		SetPerformanceFrequency(li.QuadPart);
		s_isInitialized = true;
	}
}