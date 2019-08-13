#ifndef __MARKET_H__
#define __MERKET_H__

#include"stdafx.h"

#include "Stock.h"
#include"SetStockCode.h"

#include"globedef.h"

using namespace std;
#include<vector>
#include<map>

class CMarket: public CObject 
{
public:
	// ֻ����һ��ʵ��
	CMarket(void);
	~CMarket(void);
  void Reset(void);

#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif

// interface function
public:
	// ��ʼ���г�
  bool          SaveStockCodeDataBase(void);
  void          LoadStockCodeDataBase(void);
  
  bool          UpdateOptionDataBase(void);
  void          LoadOptionDataBase(void);

  bool          UpdateTempRTData(void);

  bool          OpenSavingDayLineRecord(void);

  // ʵʱ���ݺ�������ʷ���ݶ�ȡ
  bool          CreateRTDataInquiringStr(CString& str);
  int						GetInquiringStockStr(CString& str);
  bool          GetSinaStockRTData(void);
  bool          CreateDayLineInquiringStr(CString& str, CString& strStartDay);
  bool          GetNetEaseStockDayLineData(void);


  bool          IsAStock(CStockPtr pStock);			// �Ƿ�Ϊ����A��
  bool          IsAStock(CString strStockCode);			// �Ƿ�Ϊ����A��
  bool          IsStock( CString  strStockCode, CStockPtr & pStock );	// �Ƿ�Ϊ��ȷ�Ĺ�Ʊ����

	CString			  GetStockName( CString strStockCode );

	// �õ���Ʊ����
	bool			    GetStockIndex(CString strStockCode, long & lIndex );
	// �õ���Ʊָ��
	CStockPtr			GetStockPtr(CString strStockCode);
	CStockPtr     GetStockPtr( long lIndex );

  // �洢�¹�Ʊָ�����Ծ��Ʊ��
  void          AddStockToMarket(CStockPtr pStock);

  // �õ���ƱIDָ��
  bool          GetStockIDPtr(CString strStockCode, StockIDPtr & pStockIDPtr);

	// �õ���ǰ��ʾ��Ʊ
	CStockPtr     GetShowStock( void ) noexcept { return m_pCurrentStock; }
	void					SetShowStock( CString strStockCode );
  void					SetShowStock(CStockPtr pStock);
  bool          IsCurrentStockChanged(void);

	long					GetTotalStock( void ) noexcept { return m_lTotalActiveStock; }

	long					GetMinLineOffset( CStockID sID, time_t Time );

	bool					SystemReady( void ) noexcept { return m_fSystemReady; }
  void          SetSystemReady(bool fFlag) noexcept { m_fSystemReady = fFlag; }

  bool          IsTodayStockCompiled(void) noexcept { return m_fTodayStockCompiled; }
  void          SetTodayStockCompiledFlag(bool fFlag) noexcept { m_fTodayStockCompiled = fFlag; }

	bool					SaveDayLine(CSetDayLine * psetDayLine, CSetStockCode * psetStockCode,
                            CStockPtr pStock, vector<CDayLinePtr> & vectorDayLine, bool fReversed = true );
  bool          SaveOneRecord(CSetDayLine * psetDayLine, CDayLinePtr pDayLine);

  // ʵʱ���ݴ�����������ȡ����ʵʱ���ݴ������ݿ���
  bool          SaveRTData(void);

	  // �������ݴ�����������ȡ�����������ݴ������ݿ���
  bool          SaveDayLineData(void);
 
  bool          ClearAllDayLineVector(void);

  // �Ƿ����й�Ʊ����ʷ�������ݶ������ˡ�
  bool          IsTotalStockDayLineChecked(void);

  long          CompileCurrentTradeDayStocks(long lCurrentTradeDay);
  
  bool          SaveTodayTempData(void);
  bool          LoadTodayTempData(void);

  bool          CalculateOneDayRelativeStrong(long lDay);

	bool					IsLoadSelectedStock( void ) noexcept { return m_fLoadedSelectedStock; }
	void					SetLoadSelectedStock( bool fLoad ) noexcept { m_fLoadedSelectedStock = fLoad; }

  bool          IsTodayTempRTDataLoaded(void) noexcept { return m_fTodayTempDataLoaded; }
  void          SetTodayTempRTDataLoaded(bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

  CString       GetDownLoadingStockCodeStr(void) { return m_strCurrentStockDownLoading; }
  void          SetDownLoadingStockCodeStr(CString str) { m_strCurrentStockDownLoading = str; }

  long          GetRelativeStrongStartDay(void) noexcept { return m_lRelativeStrongStartDay; }
  void          SetRelativeStrongStartDay(long lDay) noexcept { m_lRelativeStrongStartDay = lDay; }
  long          GetRelativeStrongEndDay(void) noexcept { return m_lRelativeStrongEndDay; }
  void          SetRelativeStrongEndDay(long lDay) noexcept { m_lRelativeStrongEndDay = lDay; }
  long          GetLastLoginDay(void) noexcept { return m_lLastLoginDay; }
  void          SetLastLoginDay(long lDay) noexcept { m_lLastLoginDay = lDay; }

	INT64	        GetTotalAttackBuyAmount( void );
	INT64         GetTotalAttackSellAmount( void );

  size_t        GetTotalStockMapIndexSize(void) noexcept { return m_mapChinaMarketAStock.size(); }
  long          GetTotalStockIndex(CString str) { return m_mapChinaMarketAStock.at(str); }

  //�������Ʊ��ʵʱ���ݣ�����ҵ��仯�ȡ��ɹ����߳�ClientThreadCalculatingRTDataProc���á�
  bool          CalculateRTData(void);

  //����ʵʱ��Ʊ�仯��
  bool          ProcessRTData(void);
  // ����������ʷ����
  bool					ProcessDayLineData(char * buffer, long lLength);
	bool					ProcessOneItemDayLineData(CDayLinePtr pDayLine, char *& pCurrentPos, long & lLength);
  // ��Ȼ����������ȡʵʱ���ݵ���ȫһ������Ϊ�˷�ֹ�Ժ���ܸı��Ե�ʣ����Ƿֱ�ʵ�֡�
	bool					ReadOneValue(char *& pCurrentPos, char * buffer, long& iReadNumber);
  bool          ReadOneValueExceptperiod(char *& pCurrentPos, char * buffer, long & lCounter);

  // ��ʱ���£���ɾ���������������̵߳�OnTimerha��������
  bool          SchedulingTask(void);
  bool          SchedulingTaskPerSecond(void);

private:
  // ��ʼ��
  bool            CreateTotalStockContainer(void);    // �˺����ǹ��캯����һ���֣������������á�


public :
  const CString m_strRTStockSource = _T("http://hq.sinajs.cn/list=");
  const CString m_strDayLineStockSource = _T("http://quotes.money.163.com/service/chddata.html?code=");
  const CString m_strDayLinePostfix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
  
  CStockPtr                   m_pCurrentStock;          // ��ǰ��ʾ�Ĺ�Ʊ
  char                        m_aStockCodeTemp[30];
  bool                        m_fCurrentEditStockChanged;
  bool                        m_fMarketOpened;          // �Ƿ���
  bool										    m_fGetRTStockData;        // ��ȡʵʱ���ݱ�ʶ
  bool										    m_fGetDayLineData;        // ��ȡ������ʷ���ݱ�ʶ
  int                         m_iCountDownDayLine;        // �������ݶ�ȡ��ʱ������
  int                         m_iCountDownSlowReadingRTData;  // ���ٶ�ȡʵʱ���ݼ�����

  vector<StockIDPtr>	        m_vChinaMarketAStock;             // ��ϵͳ��������й�Ʊ�أ����۴����Ƿ���ڣ�
  
  vector<StockIDPtr>          gl_vStockChoice;      // ��ѡ��Ʊ��

  clock_t                     gl_RTReadingTime;         // ÿ�ζ�ȡ����ʵʱ���ݵ�ʱ��
  clock_t                     gl_DayLineReadingTime;    // ÿ�ζ�ȡ����������ʷ���ݵ�ʱ��

  bool                        m_fPermitResetSystem;     // ��������ϵͳ��������ϻ��������еĻ�����Ҫÿ������ϵͳ

protected :
  CSetDayLine                 m_setSavingDayLineOnly;     // �˱���ר���ڴ洢���յ���������ʷ���ݣ� �ڴ洢������ʷ����֮ǰ��ʹ֮��״̬������ͬʱ�������ݿ�ʱ�ٶ�������

  map<CString, long>	        m_mapChinaMarketAStock;		// �����б���ѯ�Ĺ�Ʊ����ӳ��Ϊƫ������Ŀǰֻ����A����Ϣ��
  CString                     m_strCurrentStockDownLoading; // Ŀǰ��������������ʷ���ݵĹ�Ʊ����
  
  // Option��ѡ��
  long                        m_lRelativeStrongStartDay;
  long                        m_lRelativeStrongEndDay;
  long                        m_lLastLoginDay;            // �ϴε�¼���ڡ����������Ϊ���յĻ�������������������ʷ����

  bool                        m_fTodayStockCompiled;        // �����Ƿ�ִ���˹�Ʊ����

	map<CString, long>          m_mapActiveStockToIndex;		// ���г���Ծ�Ĺ�Ʊ����ӳ��Ϊƫ����
	vector<CStockPtr>						m_vActiveStock;									//�����Ʊ����

  long												m_lTotalActiveStock;						// �����Ʊ����
  vector<CStockPtr>::iterator m_itStock;
  bool                        m_fResetm_ItStock;    // ����m_itStock��ʶ

	vector<CStockPtr>           m_vpSelectedStock;		// ��ǰѡ��Ĺ�Ʊ
	bool												m_fLoadedSelectedStock;
	
	bool												m_fSystemReady;					// �г���ʼ̬�Ѿ����ú�
  
  bool                        m_fTodayTempDataLoaded;      //�����ݴ����ʱ�����Ƿ���ر�ʶ��

  bool                        m_fCurrentStockChanged;   // ��ǰѡ��Ĺ�Ʊ�ı���
	INT64										    m_lTotalMarketBuy;				// �����г��е�A������������
	INT64										    m_lTotalMarketSell;					// �����г��е�A������������� 
	
  bool                        m_fCheckTodayActiveStock; // �Ƿ��ѯ���ջ�Ծ��Ʊ����

  bool                        m_fUpdatedStockCodeDataBase;  //�Ƿ������������ʷ���ݿ��ʶ

private:


};

#endif

