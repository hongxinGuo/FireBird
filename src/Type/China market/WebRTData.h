///////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ���ݰ�, ������ʵʱ��Ʊ����ӿ�(hq.sinajs.cn\list=)����Ѷʵʱ��Ʊ����ӿ�Ϊ��׼
//
// Ϊ�˼��㷽�㣬���̼۵�ʹ�÷Ŵ�1000������������ʾ��ʵ���ݣ���������Ҳ��ˡ��ڴ洢ʱ��ʹ��DECIMAL(10,3)����¼�������Ƚ����ף�������ת���������͡�
// �ҵ������ɽ����ͳɽ�����ֵ�Ȳ���ʵ����ֵ��
//
// ����ʵʱ������ʽ��hq.sinajs.cn\list=sh600001,sz002389
// var hq_str_sh601006=��������·,27.55,27.25,26.91,27.55,26.20,26.91,26.92,
//                     22114263,589824680,4695,26.91,57590,26.90,14700,26.89,14300,
//                     26.88,15100,26.87,3100,26.92,8900,26.93,14230,26.94,25150,26.95,15220,26.96,2008-01-11,15:05:32,00,��;\n
//
// ����ַ������������ƴ����һ�𣬲�ͬ����������ö��Ÿ����ˣ����ճ���Ա��˼·��˳��Ŵ�0��ʼ��
// 0����������·������Ʊ���֣�
// 1����27.55�壬���տ��̼ۣ�
// 2����27.25�壬�������̼ۣ�
// 3����26.91�壬��ǰ�۸�
// 4����27.55�壬������߼ۣ�
// 5����26.20�壬������ͼۣ�
// 6����26.91�壬����ۣ�������һ�����ۣ�
// 7����26.92�壬�����ۣ�������һ�����ۣ�
// 8����22114263�壬�ɽ��Ĺ�Ʊ�������ڹ�Ʊ������һ�ٹ�Ϊ������λ��������ʹ��ʱ��ͨ���Ѹ�ֵ����һ�٣�
// 9����589824680�壬�ɽ�����λΪ��Ԫ����Ϊ��һĿ��Ȼ��ͨ���ԡ���Ԫ��Ϊ�ɽ����ĵ�λ������ͨ���Ѹ�ֵ����һ��
// 10����4695�壬����һ������4695�ɣ���47�֣�
// 11����26.91�壬����һ�����ۣ�
// 12����57590�壬�������
// 13����26.90�壬�������
// 14����14700�壬��������
// 15����26.89�壬��������
// 16����14300�壬�����ġ�
// 17����26.88�壬�����ġ�
// 18����15100�壬�����塱
// 19����26.87�壬�����塱
// 20����3100�壬����һ���걨3100�ɣ���31�֣�
// 21����26.92�壬����һ������
// (22, 23), (24, 25), (26, 27), (28, 29)�ֱ�Ϊ���������������ĵ������
// 30����2008-01-11�壬���ڣ�
// 31����15:05:32�壬ʱ�䣻
// 32: "00", δ֪���ݶ�
//
//
// ��Ѷ�������飬�������ݽӿڣ�http://qt.gtimg.cn/q=sz002818,sh600001
// 20200101�սṹ��
// v_sz000001 = "51~ƽ������~000001~15.59~15.90~15.75~1046363~518391~527971~
//              15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~
//              20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~
//              15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~0.73~-12617~15.73~9.82~12.19~
//              ~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82";\n
//
// 20230627�սṹ��
// v_sz000001="51~ƽ������~000001~11.20~11.30~11.29~249131~93995~155136~
//						 11.20~2136~11.19~7304~11.18~14676~11.17~5431~11.16~8601~11.21~1202~11.22~1156~11.23~1027~11.24~3467~11.25~2234~~
//						 20230629112612~-0.10~-0.88~11.29~11.18~11.20/249131/279525288~249131~27953~0.13~4.60~~
//						 11.29~11.18~0.97~2173.42~2173.46~0.59~12.43~10.17~0.96~29062~11.22~3.72~4.78~
//						 ~~1.34~27952.5288~0.0000~0~~GP-A~-13.01~-1.41
//						 ~4.58~10.58~0.87~15.46~9.94~-2.48~-3.32~-8.53~19405546950~19405918198~61.53~-5.60~19405546950~~~-20.72~0.00~~CNY~0~";\n
//
// 0: 51����1��  1 ��A�� 51 ��A
// 1 : ƽ������  ����
// 2 : 000001   ����
// 3 : 15.59    �ּ�
// 4 : 15.90    ����
// 5 : 15.75    ��
// 6 : 1046363  �ɽ������֣�
// 7 : 518391   ����
// 8 : 527971   ���� �������һ�н�����
// 9 : 15.58    ��һ
// 10 : 2365    ��һ�����֣�
// 11 - 18 : ��� - ����
// 19 : 15.59   ��һ
// 20 : 661     ��һ��
// 21 - 28 : ���� - ����
// 29 : ���գ�   �����ʳɽ����������˶�Ϊ�գ�����ڶ��н�����
// 30 : 20190930154003  ʱ��
// 31 : 0.31    �ǵ�
// 32 : 1.95    �ǵ� %
// 33 : 15.89   ���
// 34 : 15.57   ���
// 35 : 15.59/1046363/1645828527    �۸� / �ɽ������֣� / �ɽ��Ԫ��
// 36 : 1046363 �ɽ������֣�
// 37 : 164583  �ɽ����
// 38 : 0.54    ������
// 39 : 11.27   ��ӯ�ʣ�TTM��
// 40 : ��      ����      (��������н�����
// 41 : 15.89   ���
// 42 : 15.57   ���
// 43 : 2.01    ���
// 44 : 3025.38 ��ͨ��ֵ���ڣ�
// 45 : 3025.38 ����ֵ���ڣ�
// 46 : 1.15    �о���
// 47 : 17.49   ��ͣ��
// 48 : 14.31   ��ͣ��
// 49 ��0.73    ����
// 50 ��-12617  ί��
// 51 ��15.73   ����
// 52 �� 9.82    ��̬��ӯ��
// 53 �� 12.19   ��̬��ӯ��   ����������н�����
// 54 ��
// 55 ��
// 56 ��
// 57 ���ɽ���
// 58 ��
// 59 ��
// 60 ��
// 61 ��GP-A
// 62 ��
// 63 ��
// 64 ��
// 65 ��
// 66 ��// �����ֶκ��岻����δ�ҵ��ٷ�˵����
//
//
// ����ʵʱ���飬���ʶ˿ڣ�http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

enum {
	INVALID_RT_WEB_DATA_ = 0,
	SINA_RT_WEB_DATA_ = 1,
	TENGXUN_RT_WEB_DATA_ = 2,
	NETEASE_RT_WEB_DATA_ = 3,
};

#include<array>
using std::array;
using std::shared_ptr;
using std::string_view;

class CWebData;
using CWebDataPtr = std::shared_ptr<CWebData>;
class CWebRTData;
using CWebRTDataPtr = std::shared_ptr<CWebRTData>;

class CWebRTData final {
public:
	// ��ʼ��
	CWebRTData();
	// ������ֵ��
	CWebRTData(const CWebRTData&) = delete;
	CWebRTData& operator=(const CWebRTData&) = delete;
	CWebRTData(const CWebRTData&&) noexcept = delete;
	CWebRTData& operator=(const CWebRTData&&) noexcept = delete;
	~CWebRTData() = default;

	void Reset();

	void ParseSinaData(const string_view& svData); // ����������ʽ���ݡ���ʼ��Ϊvar hq_str_s,�������ַ�';'
	void ParseTengxunData(const string_view& svData); // ������Ѷ��ʽ���ݡ���ʼ��Ϊv_s,�������ַ�';'

	void SetDataSource(const long lDataSource) noexcept { m_lDataSource = lDataSource; }
	long GetDataSource() const noexcept { return m_lDataSource; }
	auto GetTimePoint() const noexcept { return m_tpTime; }
	void SetTimePoint(std::chrono::sys_seconds time) noexcept { m_tpTime = time; }
	time_t GetTransactionTime() const noexcept { return m_tpTime.time_since_epoch().count(); }
	void SetTransactionTime(const time_t time) noexcept { m_tpTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(time)); }
	string GetSymbol() { return m_strSymbol; }
	void SetSymbol(const string& str) { m_strSymbol = str; }
	string GetStockName() const { return m_strStockName; }
	void SetStockName(const string& str) { m_strStockName = str; }
	long GetLastClose() const noexcept { return m_lLastClose; }
	void SetLastClose(const long lValue) noexcept { m_lLastClose = lValue; }
	long GetOpen() const noexcept { return m_lOpen; }
	void SetOpen(const long lValue) noexcept { m_lOpen = lValue; }
	long GetHigh() const noexcept { return m_lHigh; }
	void SetHigh(const long lValue) noexcept { m_lHigh = lValue; }
	long GetLow() const noexcept { return m_lLow; }
	void SetLow(const long lValue) noexcept { m_lLow = lValue; }
	long GetNew() const noexcept { return m_lNew; }
	void SetNew(const long lValue) noexcept { m_lNew = lValue; }
	INT64 GetAmount() const noexcept { return m_llAmount; }
	void SetAmount(const INT64 llValue) noexcept { m_llAmount = llValue; }
	INT64 GetVolume() const noexcept { return m_llVolume; }
	void SetVolume(const INT64 llValue) noexcept { m_llVolume = llValue; }
	void SetTotalValue(const INT64 llValue) noexcept { m_llTotalValue = llValue; }
	INT64 GetTotalValue() const noexcept { return m_llTotalValue; }
	void SetCurrentValue(const INT64 llValue) noexcept { m_llCurrentValue = llValue; }
	INT64 GetCurrentValue() const noexcept { return m_llCurrentValue; }
	long GetBuy() const noexcept { return m_lBuy; }
	void SetBuy(const long lValue) noexcept { m_lBuy = lValue; }
	long GetSell() const noexcept { return m_lSell; }
	void SetSell(const long lValue) noexcept { m_lSell = lValue; }
	long GetHighLimitFromTengxun() const noexcept { return m_lHighLimitFromTengxun; }
	void SetHighLimitFromTengxun(const long lValue) noexcept { m_lHighLimitFromTengxun = lValue; }
	long GetLowLimitFromTengxun() const noexcept { return m_lLowLimitFromTengxun; }
	void SetLowLimitFromTengxun(const long lValue) noexcept { m_lLowLimitFromTengxun = lValue; }
	long GetPBuy(const int iIndex) const { return m_lPBuy.at(iIndex); }
	void SetPBuy(const int iIndex, const long lValue) { m_lPBuy.at(iIndex) = lValue; }
	long GetVBuy(const int iIndex) const { return m_lVBuy.at(iIndex); }
	void SetVBuy(const int iIndex, const long lValue) { m_lVBuy.at(iIndex) = lValue; }
	long GetPSell(const int iIndex) const { return m_lPSell.at(iIndex); }
	void SetPSell(const int iIndex, const long lValue) { m_lPSell.at(iIndex) = lValue; }
	long GetVSell(const int iIndex) const { return m_lVSell.at(iIndex); }
	void SetVSell(const int iIndex, const long lValue) { m_lVSell.at(iIndex) = lValue; }

	bool IsActive() const noexcept { return m_fActive; }
	void SetActive(const bool fFlag) noexcept { m_fActive = fFlag; }
	bool CheckNeteaseRTDataActive();
	bool CheckSinaRTDataActive();
	bool CheckTengxunRTDataActive();
	bool IsValidTime(long lDays) const;
	bool IsValidDataSource() const noexcept { return m_lDataSource != INVALID_RT_WEB_DATA_; }

	// ����
public:

protected:
	long m_lDataSource; // ʵʱ������Դ��ʶ��0���Ƿ����ݣ�1��������վ��2����Ѷ��վ��3��������վ��������
	// Serialized data
	std::chrono::sys_seconds m_tpTime; // ���׷���ʱ��system_clockʱ��
	string m_strSymbol; // ֤ȯ����, 600001.SS��002389.SZ��
	string m_strStockName; // ֤ȯ����
	long m_lOpen; // ���տ��̡���λ��0.001Ԫ
	long m_lLastClose; // �������̡���λ��0.001Ԫ
	long m_lNew; // �������¡���λ��0.001Ԫ
	long m_lHigh; // ������ߡ���λ��0.001Ԫ
	long m_lLow; // ������͡���λ��0.001Ԫ
	long m_lBuy; // ����ۡ���λ��0.001Ԫ
	long m_lSell; // �����ۡ���λ��0.001Ԫ
	long m_lHighLimitFromTengxun; // ��ͣ�ۡ���������Ŀǰֻ����Ѷʵʱ�����ܹ��ṩ��
	long m_lLowLimitFromTengxun; // ��ͣ�ۡ���������Ŀǰֻ����Ѷʵʱ�����ܹ��ṩ��
	INT64 m_llVolume; // �ܳɽ�������λ����
	INT64 m_llAmount; // �ܳɽ�����λ��Ԫ
	INT64 m_llTotalValue; // ����ֵ����λ����Ԫ
	INT64 m_llCurrentValue; // ��ͨ��ֵ����λ����Ԫ
	array<long, 5> m_lPBuy; // ���̼�1--5����λ��0.001Ԫ
	array<long, 5> m_lVBuy; // ������1--5����λ����
	array<long, 5> m_lPSell; // ���̼�1--5����λ��0.001Ԫ
	array<long, 5> m_lVSell; // ������1--5����λ: ��

	// �Ǵ洢����
	bool m_fActive; // ����Ʊ�Ƿ������Чʵʱ����
};
