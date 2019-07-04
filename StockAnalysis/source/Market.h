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
  bool            IsCurrentStockChanged(void) { return m_fCurrentStockChanged; }

	void					SetShowStock( CString strStockCode );

	long					GetTotalStock( void ) { return m_lTotalActiveStock; }

	long					GetMinLineOffset( CStockID sID, time_t Time );

	bool					MarketReady( void ) { return m_fMarketReady; }
  void          SetMarketReadyFlag(bool fFlag) { m_fMarketReady = fFlag; }

	bool					SaveDayLine(CSetDayLine * psetDayLine, CSetStockCode * psetStockCode,
                            CStockPtr pStock, vector<CDayLinePtr> & vectorDayLine, bool fReversed = true );
  bool          SaveOneRecord(CSetDayLine * psetDayLine, CDayLinePtr pDayLine);

  // ʵʱ���ݴ�����������ȡ����ʵʱ���ݴ������ݿ���
  bool          SaveRTData( CSetRealTimeData * psetDayLine );

	  // �������ݴ�����������ȡ�����������ݴ������ݿ���
  bool          SaveDayLineData(void);
  
  bool          ClearAllDayLineVector(void);

  // �Ƿ����й�Ʊ����ʷ�������ݶ������ˡ�
  bool          IsTotalStockDayLineChecked(void);

  bool          CompileCurrentTradeDayStocks(long lCurrentTradeDay);

	bool					IsLoadSelectedStock( void ) { return m_fLoadedSelectedStock; }
	void					SetLoadedSelectedStock( bool fLoad ) { m_fLoadedSelectedStock = fLoad; }

	long long	    GetTotalAttackBuyAmount( void );
	long long     GetTotalAttackSellAmount( void );

  //����ʵʱ��Ʊ�仯��
  bool          ProcessRTData(void);
  int						GetInquiringStockStr(CString& str);

  //�������Ʊ��ʵʱ���ݣ�����ҵ��仯�ȡ��ɹ����߳�ClientThreadCalculatingRTDataProc���á�
  bool          CalculateRTData(void);

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

protected :
	map<CString, long>          m_mapActiveStockToIndex;		// ���г���Ծ�Ĺ�Ʊ����ӳ��Ϊƫ����
	vector<CStockPtr>						m_vActiveStock;									//�����Ʊ����

  long												m_lTotalActiveStock;						// �����Ʊ����
  vector<CStockPtr>::iterator  m_itStock;

	vector<CStockPtr>            m_vpSelectedStock;		// ��ǰѡ��Ĺ�Ʊ
	bool												m_fLoadedSelectedStock;
	
	bool												m_fMarketReady;					// �г���ʼ̬�Ѿ����ú�

  bool                        m_fCurrentStockChanged;   // ��ǰѡ��Ĺ�Ʊ�ı���
	long long										m_lTotalMarketBuy;				// �����г��е�A������������
	long long										m_lTotalMarketSell;					// �����г��е�A������������� 
	
private:


};

#endif

