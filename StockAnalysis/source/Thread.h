#pragma once

#include"stdafx.h"

#include"SinaRTWebInquiry.h"
#include"SinaStakeRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"CrweberIndexWebInquiry.h"
#include"FinnhubWebInquiry.h"

#include"ChinaStake.h"
class CChinaMarket;
#include"AmericaStake.h"
#include"PotenDailyBriefingMarket.h"
#include"CrweberIndexMarket.h"

#include<gsl/gsl>
using namespace gsl;

// �������߳�����
// �߳��޷���Ϊ�ຯ���������У��޷���afxBeginThread()���ã����ʶ����̶߳�ֻ�ܲ���ȫ�ֺ�����ʽ���ڴ˴���
// �����߳����ڴ����ʱ����Ϊ�˼򻯳��������Ŀǰֻ������������Ϊ�̣߳�һ���Ǵ������ݿ��������һ����Զ���첽�����������ݵ�����
// �����������������̣߳��Լ���ͬ������ķ�������

// �����ȡ����

// ��ȡ����ʵʱ�����̡߳���ַ��http://hq.sinajs.cn/list=
UINT ThreadReadSinaRTData(not_null<CSinaRTWebInquiry*> pSinaRTWebData);    // �˹����̷߳���ֵΪ1������ΪpSinaWebRTData
// ��ȡ��Ѷʵʱ�����̡߳���ַ��http://qt.gtimg.cn/q=
UINT ThreadReadTengxunRTData(not_null<CTengxunRTWebInquiry*> pTengxunRTWebData);    // �˹����̷߳���ֵΪ2������ΪpTengxunWebRTData
// ��ȡ����ʵʱ�����̣߳���ַ��http://api.money.126.net/data/feed/
UINT ThreadReadNeteaseRTData(not_null<CNeteaseRTWebInquiry*> pNeteaseRTWebData); // ���̷߳���ֵΪ3������ΪpNeteaseWebRTData
// ��ȡ����������ʷ�����߳�, pParamΪ���ô��̵߳ı�����ָ�롣��ַ��http://quotes.money.163.com/service/chddata.html?code=
UINT ThreadReadNeteaseDayLine(not_null<CNeteaseDayLineWebInquiry*> pNeteaseDayLineWebData);      // �˹����̷߳���ֵΪ4������ΪpNeteaseWebDayLineData
// ��ȡcrweber.com��վ�ϵ�����ָ���̡߳���ַ��http://www.crweber.com
UINT ThreadReadCrweberIndex(not_null<CCrweberIndexWebInquiry*> pCrweberIndexWebData); // �˹����̷߳���ֵΪ5������ΪpCrweberIndexWebData
// ��ȡPoten Daily Briefing.��ַ��http://energy.poten.com/poten-daily-briefing-webpage-04/11/2018
UINT ThreadReadPotenDailyBriefing(not_null<CPotenDailyBriefingWebInquiry*> pPotenDailyBriefingWebData); // �˹����̷߳���ֵΪ6.
// ��ȡ����ʵʱ�����̡߳���ַ��http://hq.sinajs.cn/list=
UINT ThreadReadSinaStakeRTData(not_null<CSinaStakeRTWebInquiry*> pSinaStakeRTWebData);    // �˹����̷߳���ֵΪ6������ΪpSinaWebRTStakeData
// ��ȡFinnhub.io���������̡߳� ��ַ��https://finnhub.io/api/v1/
UINT ThreadReadFinnhubData(not_null<CFinnhubWebInquiry*> pFinnhubWebInquiry); // �˹����̷߳���ֵΪ7��

// ���ݿ⴦������

// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDayִ�о��������������8�������̡߳�
UINT ThreadBuildDayLineRS(not_null<CChinaMarket*> pMarket, long startCalculatingDay); // �˹����̷߳���ֵΪ11, ����Ϊ��ǰ����������
// ����������ڹ�Ʊ�������ǿ���̡߳����߳���ThreadCalculateRS�̵߳��ã����ڲ������㲻ͬ���ڵ��������ǿ�ȣ������Ч��
UINT ThreadBuildDayLineRSOfDate(not_null<CChinaMarket*> pMarket, long lDate); // �˹����̷߳���ֵҲΪ12
// �洢��ʱϵͳ״̬�߳�
UINT ThreadSaveTempRTData(not_null < CChinaMarket*> pMarket);     // �˹����̷߳���ֵΪ13
// ����ʹ洢��ǰ�����������߳�
UINT ThreadProcessTodayStock(not_null<CChinaMarket*> pMarket);     // �˹����̷߳���ֵΪ14
// �洢һ����Ʊ��������ʷ���ݡ�
UINT ThreadSaveDayLineBasicInfoOfStock(not_null<CChinaStakePtr> pStake); // �˹����̷߳���ֵΪ15, ����ΪЯ������ָ���һ���ṹָ��
// �����ݿ��ж�ȡ������ʷ�����߳�
UINT ThreadLoadDayLine(not_null<CChinaStakePtr> pStake);        // �˹����̷߳���ֵΪ16
// ά���������ݿ��̣߳���δʵ�֣�
UINT ThreadMaintainDayLineDataBase(void);   // �˹����̷߳���ֵΪ17
//���¹�Ʊ�������ݿ��߳�
UINT ThreadUpdateStakeCodeDB(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ18
//�洢��ѡ��Ʊʵʱ�����߳�
UINT ThreadSaveRTData(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ19
// ����ѡ�����ݿ�
UINT ThreadUpdateOptionDB(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ20
// �洢Poten.com�µ�����
UINT ThreadSavePotenData(not_null<CPotenDailyBriefingMarket*> pMarket); // ���̷߳���ֵΪ21
// ����ѡ�����ݿ�
UINT ThreadAppendChoicedStockDB(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ22
// �洢crweber��������
UINT ThreadSaveCrweberDB(not_null<CCrweberIndexMarket*> pMarket, not_null<CCrweberIndexPtr> pCrweberIndex); // ���̷߳���ֵΪ23
// ����Crweber���ݿ�
UINT ThreadMaintainCrweberDB(not_null<CCrweberIndexMarket*> pMarket); // ���̷߳���ֵΪ24
// ����������ʷ����
UINT ThreadBuildWeekLine(not_null<CChinaMarket*> pMarket, long lStartDate); // ���̵߳ķ���ֵΪ25
// ���ɹ�Ʊ��������ʷ����
UINT ThreadBuildWeekLineOfStock(not_null<CChinaStakePtr> pStake, long lStartDate); /// ���̵߳ķ���ֵΪ26
// �����ݿ��ȡ������ʷ����
UINT ThreadLoadWeekLine(not_null<CChinaStakePtr> pStake); // ���̷߳���ֵΪ29
// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDateִ�о��������������8�������̡߳�
UINT ThreadBuildWeekLineRS(not_null<CChinaMarket*> pMarket, long startCalculatingDate); // �˹����̷߳���ֵΪ30, ����Ϊ��ǰ����������
// ����������ڹ�Ʊ�������ǿ���̡߳����߳���ThreadCalculateRS�̵߳��ã����ڲ������㲻ͬ���ڵ��������ǿ�ȣ������Ч��
UINT ThreadBuildWeekLineRSOfDate(not_null<CChinaMarket*> pMarket, long lDate); // �˹����̷߳���ֵҲΪ31
// ���㱾�ܵ�����
UINT ThreadBuildWeekLineOfCurrentWeek(not_null<CChinaMarket*> pMarket); // �˹����̷߳���ֵΪ32
// �ؽ���ǰ�ܵ��������ݱ�
UINT ThreadBuildCurrentWeekWeekLineTable(not_null<CChinaMarket*> pMarket); // �˹����̷߳���ֵΪ33
// �洢֤ȯ���������ݿ�
UINT ThreadSaveActiveStakeCode(not_null<CChinaMarket*> pMarket); // �˹����̷߳���ֵΪ34
// �洢֤ȯ���������
UINT ThreadSaveStakeSection(not_null<CChinaMarket*> pMarket); // �˹����̷߳���ֵΪ35
// �洢һ��������Ʊ��������ʷ���ݡ�
UINT ThreadSaveAmericaStakeDayLine(not_null<CAmericaStakePtr> pStake); // �˹����̷߳���ֵΪ15, ����ΪЯ������ָ���һ���ṹָ��

//���ּ����ù����߳�
// ����10��ǿ��Ʊ������������ֻ����һ��ǿ�Ʒ壩
UINT ThreadChoice10RSStrong1StockSet(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ101
// ����10��ǿ��Ʊ�������γ���ǿ�Ʒ壩
UINT ThreadChoice10RSStrong2StockSet(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ102
// ����10��ǿ��Ʊ����ʹ���ⲿpRef�ṩ�Ĳ�����
UINT ThreadChoice10RSStrongStockSet(not_null<CChinaMarket*> pMarket, CRSReference* pRef, int iIndex); // ���̷߳���ֵΪ103
// �����Ʊ��10��ǿ�����
UINT ThreadCalculate10RSStrongStock(not_null<vector<CChinaStakePtr>*> pv10RSStrongStock, CRSReference* pRef, not_null<CChinaStakePtr> pStake); // ���̷߳���ֵΪ104
// �����Ʊ��10��ǿ�����1
UINT ThreadCalculate10RSStrong1Stock(not_null<vector<CChinaStakePtr>*> pv10RSStrongStock, not_null<CChinaStakePtr> pStake); // ���̷߳���ֵΪ105
// �����Ʊ��10��ǿ�����2
UINT ThreadCalculate10RSStrong2Stock(not_null<vector<CChinaStakePtr>*> pv10RSStrongStock, not_null<CChinaStakePtr> pStake); // ���̷߳���ֵΪ105
