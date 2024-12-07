#include"pch.h"
module;
#undef max
#include"concurrencpp/concurrencpp.h"
export module FireBird.Thread;

class CChinaStock;
using CChinaStockPtr = std::shared_ptr<CChinaStock>;
class CChinaMarket;
using CChinaMarketPtr = std::shared_ptr<CChinaMarket>;

import FireBird.RelativeStrongReference;

export {
	extern concurrencpp::runtime gl_runtime;
	extern concurrencpp::runtime gl_backgroundRuntime;
	extern int gl_concurrency_level; // ���м��������������

	// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDayִ�о��������������8�������̡߳�
	UINT ThreadBuildDayLineRS(const CChinaMarketPtr& pMarket, long startCalculatingDate); // �˹����̷߳���ֵΪ11, ����Ϊ��ǰ����������
	// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDateִ�о��������������8�������̡߳�
	UINT ThreadBuildWeekLineRS(const CChinaMarketPtr& pMarket, long startCalculatingDate); // �˹����̷߳���ֵΪ30, ����Ϊ��ǰ����������

	//���ּ����ù����߳�
	// ����10��ǿ��Ʊ����ʹ���ⲿpRef�ṩ�Ĳ�����
	UINT ThreadChoice10RSStrongStockSet(CRSReference* pRef, int iIndex); // ���̷߳���ֵΪ103
	// �����Ʊ��10��ǿ�����
	UINT ThreadCalculate10RSStrongStock(vector<CChinaStockPtr>* pv10RSStrongStock, const CRSReference* pRef, const CChinaStockPtr& pStock); // ���̷߳���ֵΪ104
	// �����Ʊ��10��ǿ�����1
	UINT ThreadCalculate10RSStrong1Stock(vector<CChinaStockPtr>* pv10RSStrongStock, const CChinaStockPtr& pStock); // ���̷߳���ֵΪ105
	// �����Ʊ��10��ǿ�����2
	UINT ThreadCalculate10RSStrong2Stock(vector<CChinaStockPtr>* pv10RSStrongStock, const CChinaStockPtr& pStock); // ���̷߳���ֵΪ106
}