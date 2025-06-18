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
#define __MAX_BACKGROUND_WORKING_THREAD__ 32

extern HANDLE gl_hFireBirdMutex;

extern std::counting_semaphore<__MAX_BACKGROUND_WORKING_THREAD__> gl_BackgroundWorkingThread; // ����̨�����߳���������

extern binary_semaphore gl_UpdateWorldMarketDB; // ���ź������ڸ���WorldMarket���ݿ�
extern binary_semaphore gl_ProcessChinaMarketRTData; // �����й��г���ʵʱ����ʱ��������ͬʱ�洢֮��
extern binary_semaphore gl_UpdateChinaMarketDB; // ���ڸ���ChinaMarket���ݿ�

extern map<int, string> gl_stringList;

extern shared_ptr<spdlog::logger> gl_dailyLogger;
extern shared_ptr<spdlog::logger> gl_traceLogger; // ÿ�ո�����־������ϵͳ����
extern shared_ptr<spdlog::logger> gl_warnLogger; // ÿ�վ�����־������ϵͳ����
extern shared_ptr<spdlog::logger> gl_dailyWebSocketLogger;
extern shared_ptr<spdlog::logger> gl_SoftwareDevelopingLogger;

extern std::chrono::sys_seconds gl_tpNow; // ���е��г�ʹ��ͬһ��Э������ʱ��Coordinated Universal Time��
inline time_t GetUTCTime() noexcept { return gl_tpNow.time_since_epoch().count(); }
inline void TestSetUTCTime(time_t time) noexcept { gl_tpNow = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(time)); }

extern int64_t gl_TiingoTotalData;
extern int64_t gl_FinnhubTotalData;
extern int64_t gl_ChinaMarketTotalData;

#define _TRACE_SCHEDULE_TASK___ // �����������
