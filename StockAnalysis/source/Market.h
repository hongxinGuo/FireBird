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
  bool          UpdateStockCodeDataBase(void);
  bool          UpdateOptionDataBase(void);

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
  void					SetShowStock(CStockPtr pStock);
  bool          IsCurrentStockChanged(void);

	void					SetShowStock( CString strStockCode );

	long					GetTotalStock( void ) noexcept { return m_lTotalActiveStock; }

	long					GetMinLineOffset( CStockID sID, time_t Time );

	bool					MarketReady( void ) noexcept { return m_fMarketReady; }
  void          SetMarketReadyFlag(bool fFlag) noexcept { m_fMarketReady = fFlag; }

	bool					SaveDayLine(CSetDayLine * psetDayLine, CSetStockCode * psetStockCode,
                            CStockPtr pStock, vector<CDayLinePtr> & vectorDayLine, bool fReversed = true );
  bool          SaveOneRecord(CSetDayLine * psetDayLine, CDayLinePtr pDayLine);

  bool          IsTodayStockCompiled(void) noexcept { return m_fTodayStockCompiled; }
  void          SetTodayStockCompiledFlag(bool fFlag) noexcept { m_fTodayStockCompiled = fFlag; }

  // ʵʱ���ݴ�����������ȡ����ʵʱ���ݴ������ݿ���
  bool          SaveRTData(void);

	  // �������ݴ�����������ȡ�����������ݴ������ݿ���
  bool          SaveDayLineData(void);
  
  bool          ClearAllDayLineVector(void);

  // �Ƿ����й�Ʊ����ʷ�������ݶ������ˡ�
  bool          IsTotalStockDayLineChecked(void);

  bool          CompileCurrentTradeDayStocks(long lCurrentTradeDay);
  bool          CalculateOneDayRelativeStrong(long lDay);

	bool					IsLoadSelectedStock( void ) noexcept { return m_fLoadedSelectedStock; }
	void					SetLoadSelectedStock( bool fLoad ) noexcept { m_fLoadedSelectedStock = fLoad; }

  CString       GetDownLoadingStockCodeStr(void) { return m_strCurrentStockDownLoading; }
  void          SetDownLoadingStockCodeStr(CString str) { m_strCurrentStockDownLoading = str; }

  long          GetRelativeStrongStartDay(void) noexcept { return m_lRelativeStrongStartDay; }
  void          SetRelativeStrongStartDay(long lDay) noexcept { m_lRelativeStrongStartDay = lDay; }
  long          GetRelativeStrongEndDay(void) noexcept { return m_lRelativeStrongEndDay; }
  void          SetRelativeStrongEndDay(long lDay) noexcept { m_lRelativeStrongEndDay = lDay; }

	INT64	        GetTotalAttackBuyAmount( void );
	INT64         GetTotalAttackSellAmount( void );


  //�������Ʊ��ʵʱ���ݣ�����ҵ��仯�ȡ��ɹ����߳�ClientThreadCalculatingRTDataProc���á�
  bool          CalculateRTData(void);

  //����ʵʱ��Ʊ�仯��
  bool          ProcessRTData(void);
  // ����������ʷ����
  bool					ProcessDayLineData(char * buffer, long lLength);
	bool					ProcessOneItemDayLineData(CDayLinePtr pDayLine, char *& pCurrentPos, long & lLength);
	bool					ReadOneValue(char *& pCurrentPos, char * buffer, long& iReadNumber);
  bool          ReadOneValueExceptperiod(char *& pCurrentPos, char * buffer, long & lCounter);

  // ��ʱ���£���ɾ���������������̵߳�OnTimerha��������
  bool          SchedulingTask(void);
  bool          SchedulingTaskPerSecond(void);

private:
  // ��ʼ��
  bool            CreateTotalStockContainer(void);    // �˺����ǹ��캯����һ���֣������������á�


public :
  const CString gl_strRTStockSource = _T("http://hq.sinajs.cn/list=");
  const CString gl_strDayLineStockSource = _T("http://quotes.money.163.com/service/chddata.html?code=");
  const CString gl_strDayLinePostfix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
  
  CStockPtr                   m_pCurrentStock;          // ��ǰ��ʾ�Ĺ�Ʊ
  char                        m_aStockCodeTemp[30];
  bool                        m_fCurrentEditStockChanged;
  bool                        m_fMarketOpened;          // �Ƿ���
  bool										    m_fGetRTStockData;
  bool										    m_fGetDayLineData;
  bool                        m_fCountDownRT;
  int                         m_iCountDownDayLine;        // �������ݶ�ȡ��ʱ������
  int                         m_iCountDownRT;

  vector<StockIDPtr>	        gl_vTotalStock;             // ��ϵͳ��������й�Ʊ�أ����۴����Ƿ���ڣ�
  size_t                      GetTotalStockMapIndexSize(void) { return gl_mapTotalStockToIndex.size(); }
  long                        GetTotalStockIndex(CString str) { return gl_mapTotalStockToIndex.at(str); }
protected :
  map<CString, long>	        gl_mapTotalStockToIndex;		// �����б���ѯ�Ĺ�Ʊ����ӳ��Ϊƫ������Ŀǰֻ����A����Ϣ��
  CString                     m_strCurrentStockDownLoading; // Ŀǰ��������������ʷ���ݵĹ�Ʊ����
  
  long                        m_lRelativeStrongStartDay;
  long                        m_lRelativeStrongEndDay;

  bool                        m_fTodayStockCompiled;        // �����Ƿ�ִ���˹�Ʊ����

	map<CString, long>          m_mapActiveStockToIndex;		// ���г���Ծ�Ĺ�Ʊ����ӳ��Ϊƫ����
	vector<CStockPtr>						m_vActiveStock;									//�����Ʊ����

  long												m_lTotalActiveStock;						// �����Ʊ����
  vector<CStockPtr>::iterator m_itStock;
  bool                        m_fResetm_ItStock;    // ����m_itStock��ʶ

	vector<CStockPtr>           m_vpSelectedStock;		// ��ǰѡ��Ĺ�Ʊ
	bool												m_fLoadedSelectedStock;
	
	bool												m_fMarketReady;					// �г���ʼ̬�Ѿ����ú�

  bool                        m_fCurrentStockChanged;   // ��ǰѡ��Ĺ�Ʊ�ı���
	INT64										    m_lTotalMarketBuy;				// �����г��е�A������������
	INT64										    m_lTotalMarketSell;					// �����г��е�A������������� 
	
  bool                        m_fCheckTodayActiveStock; // �Ƿ��ѯ���ջ�Ծ��Ʊ����


  bool                        m_fUpdatedStockCodeDataBase;

private:


};

#endif

