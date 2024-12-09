#pragma once

#include "spdlog/sinks/daily_file_sink.h"
#include"ConCurrenCppHeader.h"

#include <semaphore>
#include<map>
#include<string>
#include<memory>
using std::binary_semaphore;
using std::map;
using std::string;
using std::shared_ptr;

#define __MAX_BACKGROUND_WORKING_THREAD__ 32

extern HANDLE gl_hFireBirdMutex;

extern concurrencpp::runtime gl_runtime;
extern concurrencpp::runtime gl_backgroundRuntime;
extern int gl_concurrency_level; // ���м��������������

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

#define _TRACE_SCHEDULE_TASK___ // �����������
