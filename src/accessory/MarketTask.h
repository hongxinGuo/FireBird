// �й��г��ĸ�����
#pragma once

enum {
	// �����г�����
	CREATE_TASK__ = 1, // ���������ĳ�ʼ����

	// China market
	CHINA_MARKET_CHECK_SYSTEM_READY__ = 101, // ϵͳ��ʼ�����
	CHINA_MARKET_RESET__, // ����ϵͳ��ÿ�����Σ�ʱ��Ϊ091300��092600
	CHINA_MARKET_LOAD_TEMP_RT_DATA__, // װ�ر�����ʱ�洢��ʵʱ����
	CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, // ʵʱ���ݴ�����093000��ʼ��1130500��Ȼ���130000��150500��ÿ�μ��һ���ӣ����ó��������ӣ�
	CHINA_MARKET_SAVE_TEMP_RT_DATA__, // �洢��ʱʵʱ���ݡ���093000��ʼ��1130500��Ȼ���130000��150500��ÿ�μ�������
	CHINA_MARKET_BUILD_TODAY_DATABASE__,  // ʵʱ���ݴ���ÿ��һ�Σ�ʱ��Ϊ150500
	CHINA_MARKET_VALIDATE_TODAY_DATABASE__, // �����������Ƿ������û�еĻ�����֮

	CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__, // ʵʱ���ݴ���ÿ��һ�Σ�ʱ��Ϊ150500

	CHINA_MARKET_UPDATE_OPTION_DB__, // ÿ����Ӹ���һ��ϵͳѡ�����ݿ�
	CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__, // ÿ����Ӹ���һ�ι�Ʊ������ݿ�
	CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__, // ������ѡ�����ݿ�
	CHINA_MARKET_UPDATE_STOCK_SECTION__, // ����ȫ�ι�Ʊ���ݿ�

	CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__, // ÿʮ���Ӽ��һ�Σ��д�������������ݾʹ���

	CHINA_MARKET_LOAD_CURRENT_STOCK_DAY_LINE__,
	CHINA_MARKET_ACCESSORY_TASK__, // ��������������ȷ������ʱ������Ƿ���ٽ������ݵȵȣ�

	// World market
	WORLD_MARKET_CHECK_SYSTEM_READY__, // ϵͳ��ʼ�����
	WORLD_MARKET_RESET__,
	WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__,
	WORLD_MARKET_MONITORING_WEB_SOCKET_STATUS__,
	WORLD_MARKET_PROCESS_WEB_SOCKET_DATA__
};

extern map<long, CString> gl_mapMarketMapIndex;
extern map<CString, long> gl_mapMarketMapString;

class CMarketTask {
public:
	CMarketTask();
	CMarketTask(long lTime, long lType);
	~CMarketTask() = default;

	long GetTime() const { return m_lTime; }
	void SetTime(long lTime) { m_lTime = lTime; }

	long GetType() const { return m_lType; }
	void SetType(long lType) { m_lType = lType; }

protected:
	long m_lTime{0}; // HHMMSS��ʽ
	long m_lType{0}; // ö���������
};

using CMarketTaskPtr = shared_ptr<CMarketTask>;
