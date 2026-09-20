#include"pch.h"

#include "log.h"
#include"systemMessage.h"

using namespace std;

void logInfoDatabaseException(const string& typeName, const string& msg, const sqlpp::mysql::exception& e) {
	string s = std::format("{}'s {} exception: {}", typeName, msg, e.what());
	gl_dailyLogger->info("{}", s);
	gl_systemMessage.PushInnerSystemInformationMessage(s);
}

void logWarnDatabaseException(const string& typeName, const string& msg, const sqlpp::mysql::exception& e) {
	string s = std::format("{}'s {} exception: {}", typeName, msg, e.what());
	gl_dailyLogger->warn("{}", s);
	gl_systemMessage.PushInnerSystemInformationMessage(s);
}

void logErrorDatabaseException(const string& typeName, const string& msg, const sqlpp::mysql::exception& e) {
	string s = std::format("{}'s {} exception: {}", typeName, msg, e.what());
	gl_dailyLogger->error("{}", s);
	gl_systemMessage.PushInnerSystemInformationMessage(s);
}
