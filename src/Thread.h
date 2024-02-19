//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������߳�����
// �߳��޷���Ϊ�ຯ���������У��޷���afxBeginThread()���ã����ʶ����̶߳�ֻ�ܲ���ȫ�ֺ�����ʽ���ڴ˴���
// �����߳����ڴ����ʱ����Ϊ�˼򻯳��������Ŀǰֻ������������Ϊ�̣߳�һ���Ǵ������ݿ��������һ����Զ���첽�����������ݵ�����
// �����������������̣߳��Լ���ͬ������ķ�������
//
// ����ָ���Ƿ�����Բ�������ʽ�������߳��У������в����������һ�¡�Ŀǰʹ��vector����Ӵ�������ָ�롣
//
// todo ʹ��concurrencpp��д�����߳�
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <concurrencpp/runtime/runtime.h>

#include"RSReference.h"

#include"ClassDeclaration.h"

using gsl::not_null;

extern concurrencpp::runtime gl_runtime;

// �����ȡ����
//

// ���ݿ⴦������

// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDayִ�о��������������8�������̡߳�
UINT ThreadBuildDayLineRS(const not_null<CChinaMarketPtr>& pMarket, long startCalculatingDate); // �˹����̷߳���ֵΪ11, ����Ϊ��ǰ����������
// ����������ڹ�Ʊ�������ǿ���̡߳����߳���ThreadCalculateRS�̵߳��ã����ڲ������㲻ͬ���ڵ��������ǿ�ȣ������Ч��
UINT ThreadBuildDayLineRSOfDate(long lDate); // �˹����̷߳���ֵҲΪ12
// ����������ʷ����
UINT ThreadBuildWeekLine(const not_null<CChinaMarketPtr>& pMarket, long lStartDate); // ���̵߳ķ���ֵΪ25
// ���ɹ�Ʊ��������ʷ����
UINT ThreadBuildWeekLineOfStock(const not_null<CChinaStockPtr>& pStockInput, long lStartDate); /// ���̵߳ķ���ֵΪ26
// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDateִ�о��������������8�������̡߳�
UINT ThreadBuildWeekLineRS(const not_null<CChinaMarketPtr>& pMarket, long startCalculatingDate); // �˹����̷߳���ֵΪ30, ����Ϊ��ǰ����������
// ����������ڹ�Ʊ�������ǿ���̡߳����߳���ThreadCalculateRS�̵߳��ã����ڲ������㲻ͬ���ڵ��������ǿ�ȣ������Ч��
UINT ThreadBuildWeekLineRSOfDate(long lDate); // �˹����̷߳���ֵҲΪ31
// ���㱾�ܵ�����
UINT ThreadBuildWeekLineOfCurrentWeek(const not_null<CChinaMarketPtr>& pMarket); // �˹����̷߳���ֵΪ32
// �ؽ���ǰ�ܵ��������ݱ�
UINT ThreadBuildCurrentWeekWeekLineTable(const not_null<CChinaMarketPtr>& pMarket); // �˹����̷߳���ֵΪ33

// ����Ϊ������Ʊ�г��Ĺ����߳�
// ����Tiingo��ҵ���ࡣ
UINT ThreadUpdateTiingoIndustry(const not_null<CWorldMarketPtr>& pMarket); // �˹����̷߳���ֵΪ45
// ����SIC��ҵ���ࡣ
UINT ThreadUpdateSICIndustry(const not_null<CWorldMarketPtr>& pMarket); // �˹����̷߳���ֵΪ46
// ����Naics��ҵ���ࡣ
UINT ThreadUpdateNaicsIndustry(const not_null<CWorldMarketPtr>& pMarket); // �˹����̷߳���ֵΪ47
// ����Forex Exchange
UINT ThreadUpdateForexExchangeDB(); // �˹����̷߳���ֵΪ49

//���ּ����ù����߳�
// ����10��ǿ��Ʊ����ʹ���ⲿpRef�ṩ�Ĳ�����
UINT ThreadChoice10RSStrongStockSet(CRSReference* pRef, int iIndex); // ���̷߳���ֵΪ103
// �����Ʊ��10��ǿ�����
UINT ThreadCalculate10RSStrongStock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, const CRSReference* pRef, const not_null<CChinaStockPtr>& pStock); // ���̷߳���ֵΪ104
// �����Ʊ��10��ǿ�����1
UINT ThreadCalculate10RSStrong1Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, const not_null<CChinaStockPtr>& pStock); // ���̷߳���ֵΪ105
// �����Ʊ��10��ǿ�����2
UINT ThreadCalculate10RSStrong2Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, const not_null<CChinaStockPtr>& pStock); // ���̷߳���ֵΪ106
