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

#include"RSReference.h"
#include"ClassDeclaration.h"

#undef max
#include"concurrencpp/concurrencpp.h"
using namespace concurrencpp;

#include <concurrencpp/runtime/runtime.h>
extern concurrencpp::runtime gl_runtime;
extern concurrencpp::timer gl_timer;

using gsl::not_null;

// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDayִ�о��������������8�������̡߳�
UINT ThreadBuildDayLineRS(const not_null<CChinaMarketPtr>& pMarket, long startCalculatingDate); // �˹����̷߳���ֵΪ11, ����Ϊ��ǰ����������
// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDateִ�о��������������8�������̡߳�
UINT ThreadBuildWeekLineRS(const not_null<CChinaMarketPtr>& pMarket, long startCalculatingDate); // �˹����̷߳���ֵΪ30, ����Ϊ��ǰ����������

//���ּ����ù����߳�
// ����10��ǿ��Ʊ����ʹ���ⲿpRef�ṩ�Ĳ�����
UINT ThreadChoice10RSStrongStockSet(CRSReference* pRef, int iIndex); // ���̷߳���ֵΪ103
// �����Ʊ��10��ǿ�����
UINT ThreadCalculate10RSStrongStock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, const CRSReference* pRef, const not_null<CChinaStockPtr>& pStock); // ���̷߳���ֵΪ104
// �����Ʊ��10��ǿ�����1
UINT ThreadCalculate10RSStrong1Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, const not_null<CChinaStockPtr>& pStock); // ���̷߳���ֵΪ105
// �����Ʊ��10��ǿ�����2
UINT ThreadCalculate10RSStrong2Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, const not_null<CChinaStockPtr>& pStock); // ���̷߳���ֵΪ106
