#include"pch.h"

#include "Initialization.h"
#include"thread.h"
#include"log.h"

#include "CharSetTransfer.h"
#include "SystemMessage.h"
#include "VirtualDataSource.h"

using std::chrono::time_point_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::steady_clock;
using std::chrono::system_clock;

using std::wstring;

////////////////////////////////////////////////////////////////////////////////////////////
///
///<summery>
///
/// 每100毫秒调度的任务，负责各种主要任务
///
/// 由gl_runtime调用。
///<summery>
///
///
/////////////////////////////////////////////////////////////////////////////////////////////
void TaskSchedulePer100ms() {
	static bool s_Processing = false;
	string str = "ScheduleMarket100msTask unhandled exception founded : ";
	if (IsMarketResetting()) return;// 市场重启需要较长时间，无法并行工作，故而暂停调度。
	if (s_Processing) {
		gl_dailyLogger->warn("TaskSchedulePer100ms()发生重入");
		return;
	}
	s_Processing = true;
	auto start = time_point_cast<milliseconds>(steady_clock::now());
	try {
		// 获取系统时间戳。
		gl_tpNow = time_point_cast<seconds>(system_clock::now());
		ScheduleMarketTask();	// 调用主调度函数,由各市场调度函数执行具体任务
		//Todo: 其他各DataSource的调度，也考虑移至此处。目前各DataSource的调度，在CVirtualMarket的ScheduleTask()中。
	} catch (std::exception* e) { // 此处截获本体指针，以备处理完后删除之。
		s_Processing = false;
		str += e->what();
		gl_dailyLogger->warn(str);
		delete e; // 删除之，防止由于没有处理exception导致程序意外退出。
	} catch (CException* e) {
		s_Processing = false;
		str += "CException";
		gl_dailyLogger->warn(str);
		delete e; // 删除之，防止由于没有处理exception导致程序意外退出。
	} catch (...) {
		s_Processing = false;
		str += "Unknown exception";
		gl_dailyLogger->warn(str);
	}

	auto end = time_point_cast<milliseconds>(steady_clock::now());
	gl_systemMessage.IncreaseScheduleTaskTime((end - start).count());
	s_Processing = false;
}

////////////////////////////////////////////////////////////////////////////////////////////
///
///<summery>
///
/// 每秒调度的任务，负责完成各种辅助任务（每100毫秒调度的任务是主要任务）
///
///
///<summery>
///
///
/////////////////////////////////////////////////////////////////////////////////////////////
void TaskSchedulePerSecond() {
	try {
		CVirtualDataSource::CalcTotalBytePerSecond(); // 计算每秒读取的数据量
		gl_systemMessage.CalcScheduleTaskTimePerSecond(); // 计算每秒调度所需的时间
	} catch (std::exception* e) {	// 此处截获本体指针，以备处理完后删除之。
		string str1 = "TaskSchedulePerSecond unhandled exception founded : ";
		str1 += e->what();
		gl_systemMessage.PushErrorMessage(str1);
		gl_errorLogger->error("{}", str1);
		delete e; // 删除之，防止由于没有处理exception导致程序意外退出。
	} catch (CException* e) {	// 此处截获本体指针，以备处理完后删除之。
		WCHAR buffer[1000];
		wstring str = L"TaskSchedulePerSecond unhandled CException founded : ";
		e->GetErrorMessage(buffer, 1);
		str += buffer;
		gl_systemMessage.PushErrorMessage(W2Utf8(str));
		gl_errorLogger->error("{}", W2Utf8(str));
		delete e; // 删除之，防止由于没有处理exception导致程序意外退出。
	}
}

void TaskExitSystem() {
	// 向主窗口发送关闭窗口系统消息，通知框架窗口执行关闭任务。
	// 由于系统需要顺序关闭各项任务，故而不允许直接退出系统。
	PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), WM_SYSCOMMAND, SC_CLOSE, 0);
}
