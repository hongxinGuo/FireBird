// �й��г��ĸ�����
#pragma once

enum {
	CHINA_MARKET_CREATE_TASK__ = 1, // ���������ĳ�ʼ����
	CHINA_MARKET_RESET__, // ����ϵͳ��ÿ��һ�Σ�ʱ��Ϊ091300
	CHINA_MARKET_RESET_AGAIN__, // �ڶ�������ϵͳ��ÿ��һ�Σ�ʱ��Ϊ092500
	CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, // ʵʱ���ݴ�����093000��ʼ��1130500��Ȼ���130000��150500��ÿ�μ��һ���ӣ����ó��������ӣ�
	CHINA_MARKET_SAVE_TEMP_RT_DATA__, // �洢��ʱʵʱ���ݡ���093000��ʼ��1130500��Ȼ���130000��150500��ÿ�μ�������
	CHINA_MARKET_BUILD_TODAY_DATABASE__,  // ʵʱ���ݴ���ÿ��һ�Σ�ʱ��Ϊ150500

	CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__, // ʵʱ���ݴ���ÿ��һ�Σ�ʱ��Ϊ150500
	CHINA_MARKET_CHOICE_10_RS_STRONG_1_STOCK_SET__, // ʵʱ���ݴ���ÿ��һ�Σ�ʱ��Ϊ150500
	CHINA_MARKET_CHOICE_10_RS_STRONG_2_STOCK_SET__, // ʵʱ���ݴ���ÿ��һ�Σ�ʱ��Ϊ150500

	WORLD_MARKET_CREATE_TASK_ = 1001,
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
