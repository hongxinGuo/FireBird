////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �ص���������������Ҫν�ʵĺ����С�
//
// Ϊ������ʵ��ϸ�ڣ�����Ķ����ڶ�Ӧ��cpp�ļ��С�
//
// ���к�����Ҫ�������е����򡣱�ϵͳʹ�õ������У�һ��洢���Ǹ����ݵ�����ָ�룬�޷�ʹ��Ԫ�ر�����<������������
// ֻ��ʹ���ⶨ���ν����ʽ���ʶ���Ҫ�������к�����
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ClassDeclaration.h"

[[nodiscard]] bool CompareEPSSurprise(CEPSSurprisePtr& p1, CEPSSurprisePtr& p2);
[[nodiscard]] bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2);
[[nodiscard]] bool CompareCountryList(CCountryPtr& p1, CCountryPtr& p2);
[[nodiscard]] bool CompareInsiderTransaction(CInsiderTransactionPtr& p1, CInsiderTransactionPtr& p2);
[[nodiscard]] bool CompareInsiderSentiment(CInsiderSentimentPtr& p1, CInsiderSentimentPtr& p2);
[[nodiscard]] bool CompareFinnhubCompanyNews(CCompanyNewsPtr& p1, CCompanyNewsPtr& p2);

[[nodiscard]] bool CompareChinaStock(CChinaStockPtr p1, CChinaStockPtr p2);
[[nodiscard]] bool CompareWorldStock(CWorldStockPtr p1, CWorldStockPtr p2);