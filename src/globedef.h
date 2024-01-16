#pragma once

#ifndef __AFXWIN_H__
#error"�ڰ������ļ�֮ǰ�����Ȱ�����stdafx.h���ļ������� PCH �ļ�"
#endif

#if _MSVC_LANG < 202002L
#error "��ϵͳʹ��C++20�����ϱ�׼"
#endif

#include"SystemConstant.h"
#include"SystemConfiguration.h"
#include"SystemData.h"
#include"SystemMessage.h"

#include"ClassDeclaration.h"

using std::binary_semaphore;

extern counting_semaphore<8> gl_BackgroundWorkingThread; // ����̨�����߳���������

extern binary_semaphore gl_UpdateWorldMarketDB; // ���ź������ڸ���WorldMarket���ݿ�
extern binary_semaphore gl_ProcessChinaMarketRTData; // �����й��г���ʵʱ����ʱ��������ͬʱ�洢֮��
extern binary_semaphore gl_UpdateChinaMarketDB; // ���ڸ���ChinaMarket���ݿ�

extern time_t gl_tUTCTime; // ���е��г�ʹ��ͬһ��Э������ʱ��Coordinated Universal Time��
inline time_t GetUTCTime() noexcept { return gl_tUTCTime; }
inline void TestSetUTCTime(time_t time) noexcept { gl_tUTCTime = time; }
