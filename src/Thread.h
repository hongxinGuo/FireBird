//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������߳�����
// �߳��޷���Ϊ�ຯ���������У��޷���afxBeginThread()���ã����ʶ����̶߳�ֻ�ܲ���ȫ�ֺ�����ʽ���ڴ˴���
// �����߳����ڴ����ʱ����Ϊ�˼򻯳��������Ŀǰֻ������������Ϊ�̣߳�һ���Ǵ������ݿ��������һ����Զ���첽�����������ݵ�����
// �����������������̣߳��Լ���ͬ������ķ�������
//
// ����ָ���Ƿ�����Բ�������ʽ�������߳��У������в����������һ�¡�Ŀǰʹ��vector����Ӵ�������ָ�롣
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"RSReference.h"

#include"ClassDeclaration.h"
#include "FinnhubForexSymbol.h"
#include "FinnhubCryptoSymbol.h"

#include<gsl/gsl>
using gsl::not_null;

// �����ȡ����
//

// ���ݿ⴦������

// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDayִ�о��������������8�������̡߳�
UINT ThreadBuildDayLineRS(const not_null<CChinaMarketPtr>& pMarket, long startCalculatingDate); // �˹����̷߳���ֵΪ11, ����Ϊ��ǰ����������
// ����������ڹ�Ʊ�������ǿ���̡߳����߳���ThreadCalculateRS�̵߳��ã����ڲ������㲻ͬ���ڵ��������ǿ�ȣ������Ч��
UINT ThreadBuildDayLineRSOfDate(const not_null<CChinaMarketPtr>& pMarket, long lDate); // �˹����̷߳���ֵҲΪ12
// �洢��ʱϵͳ״̬�߳�
UINT ThreadSaveTempRTData(const CChinaMarketPtr& pMarket); // �˹����̷߳���ֵΪ13
// װ����ʱ�洢��ʵʱ�����߳�
UINT ThreadLoadTempRTData(const CChinaMarketPtr& pMarket, long lTheDate); // ���̷߳���ֵΪ19
// ����ʹ洢��ǰ�����������߳�
UINT ThreadProcessTodayStock(const not_null<CChinaMarketPtr>& pMarket); // �˹����̷߳���ֵΪ14
// �洢һ����Ʊ��������ʷ���ݡ�
UINT ThreadSaveDayLineBasicInfo(const not_null<CChinaStockPtr>& pStock); // �˹����̷߳���ֵΪ15, ����ΪЯ������ָ���һ���ṹָ��
// �����ݿ��ж�ȡ������ʷ�����߳�
UINT ThreadLoadDayLine(const not_null<CChinaStockPtr>& pStockInput); // �˹����̷߳���ֵΪ16
// ά���������ݿ��̣߳���δʵ�֣�
UINT ThreadMaintainDayLineDataBase(); // �˹����̷߳���ֵΪ17
//���¹�Ʊ�������ݿ��߳�
UINT ThreadUpdateChinaStockProfileDB(const not_null<CChinaMarketPtr>& pMarket); // ���̷߳���ֵΪ18
// ����ѡ�����ݿ�
UINT ThreadUpdateOptionDB(const not_null<CChinaMarketPtr>& pMarket); // ���̷߳���ֵΪ20
// ����ѡ�����ݿ�
UINT ThreadAppendChosenStockDB(const CChinaMarketPtr& pMarket); // ���̷߳���ֵΪ22
// ����������ʷ����
UINT ThreadBuildWeekLine(const not_null<CChinaMarketPtr>& pMarket, long lStartDate); // ���̵߳ķ���ֵΪ25
// ���ɹ�Ʊ��������ʷ����
UINT ThreadBuildWeekLineOfStock(const not_null<CChinaStockPtr>& pStockInput, long lStartDate); /// ���̵߳ķ���ֵΪ26
// �����ݿ��ȡ������ʷ����
UINT ThreadLoadWeekLine(const not_null<CChinaStockPtr>& pStockInput); // ���̷߳���ֵΪ29
// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDateִ�о��������������8�������̡߳�
UINT ThreadBuildWeekLineRS(const not_null<CChinaMarketPtr>& pMarket, long startCalculatingDate); // �˹����̷߳���ֵΪ30, ����Ϊ��ǰ����������
// ����������ڹ�Ʊ�������ǿ���̡߳����߳���ThreadCalculateRS�̵߳��ã����ڲ������㲻ͬ���ڵ��������ǿ�ȣ������Ч��
UINT ThreadBuildWeekLineRSOfDate(const not_null<CChinaMarketPtr>& pMarket, long lDate); // �˹����̷߳���ֵҲΪ31
// ���㱾�ܵ�����
UINT ThreadBuildWeekLineOfCurrentWeek(const not_null<CChinaMarketPtr>& pMarket); // �˹����̷߳���ֵΪ32
// �ؽ���ǰ�ܵ��������ݱ�
UINT ThreadBuildCurrentWeekWeekLineTable(const not_null<CChinaMarketPtr>& pMarket); // �˹����̷߳���ֵΪ33
// �洢֤ȯ���������
UINT ThreadSaveStockSection(const not_null<CChinaMarketPtr>& pMarket); // �˹����̷߳���ֵΪ35

// ����Ϊ������Ʊ�г��Ĺ����߳�
// ��������������ֹ�����߳�
UINT ThreadUpdateWorldMarketStockProfileDB(const CWorldMarketPtr& pMarket); // �˹����̷߳���ֵΪ37
// ����Forex��������
UINT ThreadUpdateForexDayLineDB(const not_null<CForexSymbolPtr>& pSymbol); // �˹����̵߳ķ���ֵδ38
// ����Forex�����
UINT ThreadUpdateForexSymbolDB(); // �˹����̷߳���ֵΪ39
// ����CountryList�����
UINT ThreadUpdateCountryListDB(); // �˹����̷߳���ֵΪ40
// ����EPSSurprise��
UINT ThreadUpdateEPSSurpriseDB(const not_null<CWorldStockPtr>& pStockInput); // �˹����̷߳���ֵΪ41
// ����������Ʊ��������ʷ���ݡ�
UINT ThreadUpdateWorldStockDayLineDB(const not_null<CWorldMarketPtr>& pMarket); // �˹����̷߳���ֵΪ42
// �������������ؽ���ֹ����
UINT ThreadUpdateWorldStockDayLineStartEndDate(const CWorldMarketPtr& pMarket); // �˹����̷߳���ֵΪ43
// ����Tiingo��Ʊ����
UINT ThreadUpdateTiingoStockDB(const not_null<CWorldMarketPtr>& pMarket); // �˹����̷߳���ֵΪ44
// ����Tiingo��ҵ���ࡣ
UINT ThreadUpdateTiingoIndustry(const not_null<CWorldMarketPtr>& pMarket); // �˹����̷߳���ֵΪ45
// ����SIC��ҵ���ࡣ
UINT ThreadUpdateSICIndustry(const not_null<CWorldMarketPtr>& pMarket); // �˹����̷߳���ֵΪ46
// ����Naics��ҵ���ࡣ
UINT ThreadUpdateNaicsIndustry(const not_null<CWorldMarketPtr>& pMarket); // �˹����̷߳���ֵΪ47
// ����InsiderTransaction
UINT ThreadUpdateInsiderTransactionDB(const CWorldMarketPtr& pMarket); // �˹����̷߳���ֵΪ48
// ����Forex Exchange
UINT ThreadUpdateForexExchangeDB(const not_null<CWorldMarketPtr>& pMarket); // �˹����̷߳���ֵΪ49
// ����Economic Calender
UINT ThreadUpdateEconomicCalendarDB(); // �˹����̷߳���ֵΪ50
// ����Crypto Exchange
UINT ThreadUpdateCryptoExchangeDB(const not_null<CWorldMarketPtr>& pMarket); // �˹����̷߳���ֵΪ51
// ����Crypto�����
UINT ThreadUpdateFinnhubCryptoSymbolDB(); // �˹����̷߳���ֵΪ52
// ����Crypto��������
UINT ThreadUpdateCryptoDayLineDB(const not_null<CFinnhubCryptoSymbolPtr>& pSymbol); // �˹����̵߳ķ���ֵδ53
// ����Tiingo crypto���뼯��
UINT ThreadUpdateTiingoCryptoSymbolDB(const not_null<CWorldMarketPtr>& pMarket); // �˹����̷߳���ֵΪ54
// ����Finnhub basic financial
UINT ThreadUpdateBasicFinancialDB(const not_null<CWorldMarketPtr>& pMarket); // �˹����̷߳���ֵΪ55
// ����InsiderSentiment
UINT ThreadUpdateInsiderSentimentDB(const not_null<CWorldMarketPtr>& pMarket); // �˹����̷߳���ֵΪ58
// ���¹�˾����
UINT ThreadUpdateCompanyNewsDB(const not_null<CWorldMarketPtr>& pMarket); // �˹����̷߳���ֵΪ59
// ����EPSSurprise��
UINT ThreadUpdateEPSSurpriseDB2(const CWorldMarketPtr& pMarket); // �˹����̷߳���ֵΪ60

//���ּ����ù����߳�
// ����10��ǿ��Ʊ������������ֻ����һ��ǿ�Ʒ壩
UINT ThreadChoice10RSStrong1StockSet(const not_null<CChinaMarketPtr>& pMarket); // ���̷߳���ֵΪ101
// ����10��ǿ��Ʊ�������γ���ǿ�Ʒ壩
UINT ThreadChoice10RSStrong2StockSet(const not_null<CChinaMarketPtr>& pMarket); // ���̷߳���ֵΪ102
// ����10��ǿ��Ʊ����ʹ���ⲿpRef�ṩ�Ĳ�����
UINT ThreadChoice10RSStrongStockSet(const not_null<CChinaMarketPtr>& pMarket, CRSReference* pRef, int iIndex); // ���̷߳���ֵΪ103
// �����Ʊ��10��ǿ�����
UINT ThreadCalculate10RSStrongStock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, const CRSReference* pRef, const not_null<CChinaStockPtr>& pStock); // ���̷߳���ֵΪ104
// �����Ʊ��10��ǿ�����1
UINT ThreadCalculate10RSStrong1Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, const not_null<CChinaStockPtr>& pStock); // ���̷߳���ֵΪ105
// �����Ʊ��10��ǿ�����2
UINT ThreadCalculate10RSStrong2Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, const not_null<CChinaStockPtr>& pStock); // ���̷߳���ֵΪ106
// ���䲢����chinaMarket��ʵʱ����
UINT ThreadDistributeAndCalculateRTData(const CChinaMarketPtr& pMarket); // ���̷߳���ֵΪ108
