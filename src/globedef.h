#pragma once

#ifndef __AFXWIN_H__
#error"�ڰ������ļ�֮ǰ�����Ȱ�����stdafx.h���ļ������� PCH �ļ�"
#endif

#if _MSVC_LANG < 202002L
#error "��ϵͳʹ��C++20�����ϱ�׼"
#endif

#include <semaphore>
using std::binary_semaphore;

#include"SystemConstantChinaMarket.h"
#include"SystemConfiguration.h"
#include"SystemData.h"
#include"SystemMessage.h"

#include"ClassDeclaration.h"

#include "spdlog/sinks/daily_file_sink.h"

extern HANDLE gl_hFireBirdMutex;

extern std::counting_semaphore<8> gl_BackgroundWorkingThread; // ����̨�����߳���������

extern binary_semaphore gl_UpdateWorldMarketDB; // ���ź������ڸ���WorldMarket���ݿ�
extern binary_semaphore gl_ProcessChinaMarketRTData; // �����й��г���ʵʱ����ʱ��������ͬʱ�洢֮��
extern binary_semaphore gl_UpdateChinaMarketDB; // ���ڸ���ChinaMarket���ݿ�

extern map<int, string> gl_stringList;

extern shared_ptr<spdlog::logger> gl_dailyLogger;
extern shared_ptr<spdlog::logger> gl_traceLogger; // ÿ�ո�����־������ϵͳ����
extern shared_ptr<spdlog::logger> gl_dailyWebSocketLogger;
extern shared_ptr<spdlog::logger> gl_SoftwareDevelopingLogger;

extern time_t gl_tUTCTime; // ���е��г�ʹ��ͬһ��Э������ʱ��Coordinated Universal Time��
inline time_t GetUTCTime() noexcept { return gl_tUTCTime; }
inline void TestSetUTCTime(time_t time) noexcept { gl_tUTCTime = time; }

#define _TRACE_SCHEDULE_TASK___ // �����������
