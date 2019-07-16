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

#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif

// interface function
public:
	// ��ʼ���г�

  bool            IsAStock(CStockPtr pStock);			// �Ƿ�Ϊ����A��
  bool            IsAStock(CString strStockCode);			// �Ƿ�Ϊ����A��
  bool            IsStock( CString  strStockCode, CStockPtr & pStock );	// �Ƿ�Ϊ��ȷ�Ĺ�Ʊ����

	CString			    GetStockName( CString strStockCode );

	// �õ���Ʊ����
	bool			      GetStockIndex(CString strStockCode, long & lIndex );
	// �õ���Ʊָ��
	CStockPtr			  GetStockPtr(CString strStockCode);
	CStockPtr       GetStockPtr( long lIndex );

  // �õ���ƱIDָ��
  bool            GetStockIDPtr(CString strStockCode, StockIDPtr & pStockIDPtr);

	// �õ���ǰ��ʾ��Ʊ
	CStockPtr       GetShowStock( void ) { return m_pCurrentStock; }
  void					  SetShowStock(CStockPtr pStock);
  bool            IsCurrentStockChanged(void) {
    if (m_fCurrentStockChanged) { m_fCurrentStockChanged = false; return true; }
    else return false;
  }

	void					SetShowStock( CString strStockCode );

	long					GetTotalStock( void ) { return m_lTotalActiveStock; }

	long					GetMinLineOffset( CStockID sID, time_t Time );

	bool					MarketReady( void ) { return m_fMarketReady; }
  void          SetMarketReadyFlag(bool fFlag) { m_fMarketReady = fFlag; }

	bool					SaveDayLine(CSetDayLine * psetDayLine, CSetStockCode * psetStockCode,
                            CStockPtr pStock, vector<CDayLinePtr> & vectorDayLine, bool fReversed = true );
  bool          SaveOneRecord(CSetDayLine * psetDayLine, CDayLinePtr pDayLine);

  bool          IsTodayStockCompiled(void) { return m_fTodayStockCompiled; }
  void          SetTodayStockCompiledFlag(bool fFlag) { m_fTodayStockCompiled = fFlag; }

  // ʵʱ���ݴ�����������ȡ����ʵʱ���ݴ������ݿ���
  bool          SaveRTData(void);

	  // �������ݴ�����������ȡ�����������ݴ������ݿ���
  bool          SaveDayLineData(void);
  
  bool          ClearAllDayLineVector(void);

  // �Ƿ����й�Ʊ����ʷ�������ݶ������ˡ�
  bool          IsTotalStockDayLineChecked(void);

  bool          CompileCurrentTradeDayStocks(long lCurrentTradeDay);

	bool					IsLoadSelectedStock( void ) { return m_fLoadedSelectedStock; }
	void					SetLoadSelectedStock( bool fLoad ) { m_fLoadedSelectedStock = fLoad; }

  CString       GetDownLoadingStockCodeStr(void) { return m_strCurrentStockDownLoading; }
  void          SetDownLoadingStockCodeStr(CString str) { m_strCurrentStockDownLoading = str; }

  long          GetRelativeStrongStartDay(void) { return m_lRelativeStrongStartDay; }
  void          SetRelativeStrongStartDay(long lDay) { m_lRelativeStrongStartDay = lDay; }
  long          GetRelativeStrongEndDay(void) { return m_lRelativeStrongEndDay; }
  void          SetRelativeStrongEndDay(long lDay) { m_lRelativeStrongEndDay = lDay; }

	INT64	    GetTotalAttackBuyAmount( void );
	INT64     GetTotalAttackSellAmount( void );


  //�������Ʊ��ʵʱ���ݣ�����ҵ��仯�ȡ��ɹ����߳�ClientThreadCalculatingRTDataProc���á�
  bool          CalculateRTData(void);

  //����ʵʱ��Ʊ�仯��
  bool          ProcessRTData(void);
  int						GetInquiringStockStr(CString& str);
  // ����������ʷ����
  bool					ProcessDayLineData(char * buffer, long lLength);
	bool					ProcessOneItemDayLineData(CDayLinePtr pDayLine, char *& pCurrentPos, long & lLength);
	bool					ReadOneValue(char *& pCurrentPos, char * buffer, long& iReadNumber);
  bool          ReadOneValueExceptperiod(char *& pCurrentPos, char * buffer, long & lCounter);

  // ���Ⱥ������������еĶ�ʱ����������ע�̵߳�OnTimerha��������
  bool          SchedulingTask(void);

public :
  CStockPtr                   m_pCurrentStock;          // ��ǰ��ʾ�Ĺ�Ʊ
  char                        m_aStockCodeTemp[30];
  bool                        m_fCurrentEditStockChanged;
  bool                        m_fMarketOpened;          // �Ƿ���
protected :
  CString                     m_strCurrentStockDownLoading; // Ŀǰ��������������ʷ���ݵĹ�Ʊ����
  
  long                        m_lRelativeStrongStartDay;
  long                        m_lRelativeStrongEndDay;

  bool                        m_fTodayStockCompiled;        // �����Ƿ�ִ���˹�Ʊ����

	map<CString, long>          m_mapActiveStockToIndex;		// ���г���Ծ�Ĺ�Ʊ����ӳ��Ϊƫ����
	vector<CStockPtr>						m_vActiveStock;									//�����Ʊ����

  long												m_lTotalActiveStock;						// �����Ʊ����
  vector<CStockPtr>::iterator m_itStock;

	vector<CStockPtr>           m_vpSelectedStock;		// ��ǰѡ��Ĺ�Ʊ
	bool												m_fLoadedSelectedStock;
	
	bool												m_fMarketReady;					// �г���ʼ̬�Ѿ����ú�

  bool                        m_fCurrentStockChanged;   // ��ǰѡ��Ĺ�Ʊ�ı���
	INT64										m_lTotalMarketBuy;				// �����г��е�A������������
	INT64										m_lTotalMarketSell;					// �����г��е�A������������� 
	
private:


};

#endif

