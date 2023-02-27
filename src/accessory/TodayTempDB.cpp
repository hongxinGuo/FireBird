#include "pch.h"
#include "TodayTempDB.h"

CTodayTempDB::CTodayTempDB() {
	if (!databaseTemp.IsOpen()) {
		OpenDatabase();
	}
}

CTodayTempDB::~CTodayTempDB() {
	if (databaseTemp.IsOpen()) {
		databaseTemp.Close();
	}
}

bool CTodayTempDB::DeleteCurrentContent() {
	if (!gl_systemStatus.IsWorkingMode()) {
		ASSERT(0); // 由于处理实际数据库，故不允许测试此函数
		exit(1); //退出系统
	}
	if (!databaseTemp.IsOpen()) {
		OpenDatabase();
	}
	databaseTemp.BeginTrans();
	databaseTemp.ExecuteSQL(_T("TRUNCATE `chinamarket`.`today`;"));
	databaseTemp.CommitTrans();
	return true;
}

void CTodayTempDB::OpenDatabase() {
	databaseTemp.Open(_T("ChinaMarket"), FALSE, FALSE, _T("ODBC;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
}
