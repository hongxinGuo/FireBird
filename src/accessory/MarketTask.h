// �г��ĸ�����
#pragma once

#include <concurrencpp/timers/timer.h>
using concurrencpp::timer;

////////////////////////////////////////////////////////////////////////////////////////////
//
// ������Ҫ��ʱ���ȵ����񣬽�����Ӧ������ţ���timer����gl_aTimer�б�����Ӧ��λ�á�
//
///////////////////////////////////////////////////////////////////////////////////////////
enum {
	// �����г�����
	GENERAL_TASK_PER_100MS__ = 1, // ÿ100����ִ��һ�ε�ͨ���߳�����
	GENERAL_TASK_PER_SECOND__, // ÿ����ִ��һ�ε�ͨ���߳�����
	RELOAD_SYSTEM__, // ����ϵͳ���ɼ�س�������װ��ϵͳ�������ܹ���ֹ�������ർ��ϵͳ������

	// China market
	CHINA_MARKET_CREATE_TASK__, // ������������ĳ�ʼ����ÿ��240000
	CHINA_MARKET_CHECK_SYSTEM_READY__, // ϵͳ��ʼ�����
	CHINA_MARKET_RESET__, // ����ϵͳ��ÿ�����Σ�ʱ��Ϊ091300��092600
	CHINA_MARKET_LOAD_TEMP_RT_DATA__, // װ�ر�����ʱ�洢��ʵʱ����
	CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, // ʵʱ���ݴ�����093000��ʼ��1130500��Ȼ���130000��150500��ÿ�μ��һ���ӣ����ó��������ӣ�
	CHINA_MARKET_SAVE_TEMP_RT_DATA__, // �洢��ʱʵʱ���ݡ���093000��ʼ��1130500��Ȼ���130000��150500��ÿ�μ�������
	CHINA_MARKET_BUILD_TODAY_DATABASE__,  // ʵʱ���ݴ���ÿ��һ�Σ�ʱ��Ϊ150500
	CHINA_MARKET_VALIDATE_TODAY_DATABASE__, //not implemented �����������Ƿ������û�еĻ�����֮

	CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__, // ʵʱ���ݴ���ÿ��һ�Σ�ʱ��Ϊ150500

	CHINA_MARKET_UPDATE_OPTION_DB__, // ÿ����Ӹ���һ��ϵͳѡ�����ݿ�
	CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__, // ÿ����Ӹ���һ�ι�Ʊ������ݿ�
	CHINA_MARKET_UPDATE_STOCK_SECTION__, // ����ȫ�ι�Ʊ���ݿ�

	CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, // ÿʮ���Ӽ��һ�Σ��д�������������ݾʹ���

	CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__, // ÿ�����ظ�ִ�еĸ���������ȷ������ʱ������Ƿ���ٽ������ݵȵȣ�
	CHINA_MARKET_PREPARING_MARKET_OPEN__, // �ڹ��п��̣�092959��ǰ����Ҫ��׼��

	// China market��ʱ����
	CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__, //Note ��ʱ���� ������ѡ�����ݿ�
	CHINA_MARKET_LOAD_CURRENT_STOCK_DAY_LINE__, //Note ��ʱ����װ�ص�ǰ��Ʊ����������
	CHINA_MARKET_UPDATE_CURRENT_STOCK__, //Note ��ʱ���񡣸��µ�ǰ��Ʊ��

	// World market
	WORLD_MARKET_CREATE_TASK__, // ������������ĳ�ʼ����ÿ��240000
	WORLD_MARKET_CHECK_SYSTEM_READY__, // ϵͳ��ʼ�����
	WORLD_MARKET_RESET__,	// world market���á�Ĭ�����г�ʱ��170000
	WORLD_MARKET_UPDATE_DB__, // �����г������ݿ⡣ÿһ����һ�Ρ�
	WORLD_MARKET_MONITOR_ALL_WEB_SOCKET__, // ����WebSocket״̬��ÿ����һ��
	WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__,	// �����WebSocket���յ������ݡ�ÿ����һ��

	WORLD_MARKET_CONNECT_FINNHUB_WEB_SOCKET__, // �Զ�����finnhub web socket
	// World market��ʱ����

	//
	TASK_END
};

extern array<timer, TASK_END> gl_aTimer; // timer���С����е��������ʹ�õ���timer�Ļ�������Ӧ��timer�洢�ڴ������С�

extern map<long, string> gl_mapMarketMapIndex;

class CMarketTask {
public:
	CMarketTask();
	CMarketTask(long lTime, long lType);
	~CMarketTask() = default;

	long GetTime() const { return m_lTime; }
	void SetTime(const long lTime) { m_lTime = lTime; }

	long GetType() const { return m_lType; }
	void SetType(const long lType) { m_lType = lType; }

protected:
	long m_lTime{0}; // HHMMSS��ʽ
	long m_lType{0}; // ö���������
};

using CMarketTaskPtr = shared_ptr<CMarketTask>;
