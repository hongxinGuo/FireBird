//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������߳�����
// �߳��޷���Ϊ�ຯ���������У��޷���afxBeginThread()���ã����ʶ����̶߳�ֻ�ܲ���ȫ�ֺ�����ʽ���ڴ˴���
// �����߳����ڴ�����ʱ����Ϊ�˼򻯳��������Ŀǰֻ������������Ϊ�̣߳�һ���Ǵ������ݿ��������һ����Զ���첽�����������ݵ�����
// �����������������̣߳��Լ���ͬ������ķ�������
//
// ����ָ���Ƿ�����Բ�������ʽ�������߳��У������в����������һ�¡�Ŀǰʹ��vector����Ӵ�������ָ�롣
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"RSReference.h"

#include"ClassDeclaration.h"

#include<gsl/gsl>
using gsl::not_null;

// �����ȡ����
//
// VirtualWebInquiry�̡߳�ͨ�������ȡ�߳�
UINT ThreadReadVirtualWebData(not_null<CVirtualWebInquiry*> pVirtualWebData); // ���̷߳���ֵΪ1

// ���ݿ⴦������

// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDayִ�о��������������8�������̡߳�
UINT ThreadBuildDayLineRS(not_null<CChinaMarket*> pMarket, long startCalculatingDay); // �˹����̷߳���ֵΪ11, ����Ϊ��ǰ����������
// ����������ڹ�Ʊ�������ǿ���̡߳����߳���ThreadCalculateRS�̵߳��ã����ڲ������㲻ͬ���ڵ��������ǿ�ȣ������Ч��
UINT ThreadBuildDayLineRSOfDate(not_null<CChinaMarket*> pMarket, long lDate); // �˹����̷߳���ֵҲΪ12
// �洢��ʱϵͳ״̬�߳�
UINT ThreadSaveTempRTData(not_null<CChinaMarket*> pMarket); // �˹����̷߳���ֵΪ13
// ����ʹ洢��ǰ�����������߳�
UINT ThreadProcessTodayStock(not_null<CChinaMarket*> pMarket); // �˹����̷߳���ֵΪ14
// �洢һ����Ʊ��������ʷ���ݡ�
UINT ThreadSaveDayLineBasicInfoOfStock(not_null<CChinaStock*> pStock); // �˹����̷߳���ֵΪ15, ����ΪЯ������ָ���һ���ṹָ��
// �����ݿ��ж�ȡ������ʷ�����߳�
UINT ThreadLoadDayLine(not_null<CChinaStock*> pStock); // �˹����̷߳���ֵΪ16
// ά���������ݿ��̣߳���δʵ�֣�
UINT ThreadMaintainDayLineDataBase(void); // �˹����̷߳���ֵΪ17
//���¹�Ʊ�������ݿ��߳�
UINT ThreadUpdateStockCodeDB(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ18
//�洢��ѡ��Ʊʵʱ�����߳�
UINT ThreadSaveRTData(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ19
// ����ѡ�����ݿ�
UINT ThreadUpdateOptionDB(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ20
// ����ѡ�����ݿ�
UINT ThreadAppendChosenStockDB(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ22
// ����������ʷ����
UINT ThreadBuildWeekLine(not_null<CChinaMarket*> pMarket, long lStartDate); // ���̵߳ķ���ֵΪ25
// ���ɹ�Ʊ��������ʷ����
UINT ThreadBuildWeekLineOfStock(not_null<CChinaStock*> pStock, long lStartDate); /// ���̵߳ķ���ֵΪ26
// �����ݿ��ȡ������ʷ����
UINT ThreadLoadWeekLine(not_null<CChinaStock*> pStock); // ���̷߳���ֵΪ29
// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDateִ�о��������������8�������̡߳�
UINT ThreadBuildWeekLineRS(not_null<CChinaMarket*> pMarket, long startCalculatingDate); // �˹����̷߳���ֵΪ30, ����Ϊ��ǰ����������
// ����������ڹ�Ʊ�������ǿ���̡߳����߳���ThreadCalculateRS�̵߳��ã����ڲ������㲻ͬ���ڵ��������ǿ�ȣ������Ч��
UINT ThreadBuildWeekLineRSOfDate(not_null<CChinaMarket*> pMarket, long lDate); // �˹����̷߳���ֵҲΪ31
// ���㱾�ܵ�����
UINT ThreadBuildWeekLineOfCurrentWeek(not_null<CChinaMarket*> pMarket); // �˹����̷߳���ֵΪ32
// �ؽ���ǰ�ܵ��������ݱ�
UINT ThreadBuildCurrentWeekWeekLineTable(not_null<CChinaMarket*> pMarket); // �˹����̷߳���ֵΪ33
// �洢֤ȯ���������
UINT ThreadSaveStockSection(not_null<CChinaMarket*> pMarket); // �˹����̷߳���ֵΪ35

// ����Ϊ������Ʊ�г��Ĺ����߳�
// ��������������ֹ�����߳�
UINT ThreadUpdateWorldMarketStockProfileDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ37
// ����Forex��������
UINT ThreadUpdateForexDayLineDB(not_null<CFinnhubForexSymbol*> pSymbol); // �˹����̵߳ķ���ֵδ38
// ����Forex�����
UINT ThreadUpdateForexSymbolDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ39
// ����CountryList�����
UINT ThreadUpdateCountryListDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ40
// ����EPSSurprise��
UINT ThreadUpdateEPSSurpriseDB(not_null<CWorldStock*> pStock); // �˹����̷߳���ֵΪ41
// ����������Ʊ��������ʷ���ݡ�
UINT ThreadUpdateWorldStockDayLineDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ42
// �������������ؽ���ֹ����
UINT ThreadUpdateWorldStockDayLineStartEndDate(CWorldMarket* pMarket); // �˹����̷߳���ֵΪ43
// ����Tiingo��Ʊ����
UINT ThreadUpdateTiingoStockDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ44
// ����Tiingo��ҵ���ࡣ
UINT ThreadUpdateTiingoIndustry(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ45
// ����SIC��ҵ���ࡣ
UINT ThreadUpdateSICIndustry(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ46
// ����Naics��ҵ���ࡣ
UINT ThreadUpdateNaicsIndustry(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ47
// ����InsiderTransaction
UINT ThreadUpdateInsiderTransactionDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ48
// ����Forex Exchange
UINT ThreadUpdateForexExchangeDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ49
// ����Economic Calender
UINT ThreadUpdateEconomicCalendarDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ50
// ����Crypto Exchange
UINT ThreadUpdateCryptoExchangeDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ51
// ����Crypto�����
UINT ThreadUpdateFinnhubCryptoSymbolDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ52
// ����Crypto��������
UINT ThreadUpdateCryptoDayLineDB(not_null<CFinnhubCryptoSymbol*> pSymbol); // �˹����̵߳ķ���ֵδ53
// ����Tiingo crypto���뼯��
UINT ThreadUpdateTiingoCryptoSymbolDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ54
// ����Finnhub basic financial
UINT ThreadUpdateBasicFinancialDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ55
// ����Finnhub basic financial annual data
UINT ThreadUpdateBasicFinancialAnnualDB(vector<CWorldStockPtr> vStock); // �˹����̷߳���ֵΪ56
// ����Finnhub basic financial quarterly data
UINT ThreadUpdateBasicFinancialQuarterlyDB(vector<CWorldStockPtr> vStock); // �˹����̷߳���ֵΪ57
// ����InsiderSentiment
UINT ThreadUpdateInsiderSentimentDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ58
// ���¹�˾����
UINT ThreadUpdateCompanyNewsDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ59

//���ּ����ù����߳�
// ����10��ǿ��Ʊ������������ֻ����һ��ǿ�Ʒ壩
UINT ThreadChoice10RSStrong1StockSet(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ101
// ����10��ǿ��Ʊ�������γ���ǿ�Ʒ壩
UINT ThreadChoice10RSStrong2StockSet(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ102
// ����10��ǿ��Ʊ����ʹ���ⲿpRef�ṩ�Ĳ�����
UINT ThreadChoice10RSStrongStockSet(not_null<CChinaMarket*> pMarket, CRSReference* pRef, int iIndex); // ���̷߳���ֵΪ103
// �����Ʊ��10��ǿ�����
UINT ThreadCalculate10RSStrongStock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, CRSReference* pRef, not_null<CChinaStockPtr> pStock); // ���̷߳���ֵΪ104
// �����Ʊ��10��ǿ�����1
UINT ThreadCalculate10RSStrong1Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, not_null<CChinaStockPtr> pStock); // ���̷߳���ֵΪ105
// �����Ʊ��10��ǿ�����2
UINT ThreadCalculate10RSStrong2Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, not_null<CChinaStockPtr> pStock); // ���̷߳���ֵΪ106
// ����chinaMarket��ʵʱ����
UINT ThreadProcessRTData(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ107

// ��̨�����߳�
// WebSource ParseAndSaveWebData�̡߳� ���̵߳ķ���ֵΪ203
UINT ThreadWebSourceParseAndStoreWebData(CVirtualDataSource* pDataSource, CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData);