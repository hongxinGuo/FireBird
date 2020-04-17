#pragma once

#include"stdafx.h"

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"CrweberIndexWebInquiry.h"

#include"ChinaStock.h"
class CChinaMarket;
#include"PotenDailyBriefingMarket.h"

// �������߳�����
// �߳��޷���Ϊ�ຯ���������У��޷���afxBeginThread()���ã����ʶ����̶߳�ֻ�ܲ���ȫ�ֺ�����ʽ���ڴ˴���
// �����߳����ڴ����ʱ����Ϊ�˼򻯳��������Ŀǰֻ������������Ϊ�̣߳�һ���Ǵ������ݿ��������һ����Զ���첽�����������ݵ�����
// �����������������̣߳��Լ���ͬ������ķ�������

// �����ȡ����

// ��ȡ����ʵʱ�����̡߳���ַ��http://hq.sinajs.cn/list=
UINT ThreadReadSinaRTData(CSinaRTWebInquiry* pSinaRTWebData);    // �˹����̷߳���ֵΪ1������ΪpSinaWebRTData
// ��ȡ��Ѷʵʱ�����̡߳���ַ��http://qt.gtimg.cn/q=
UINT ThreadReadTengxunRTData(CTengxunRTWebInquiry* pTengxunRTWebData);    // �˹����̷߳���ֵΪ2������ΪpTengxunWebRTData
// ��ȡ����ʵʱ�����̣߳���ַ��http://api.money.126.net/data/feed/
UINT ThreadReadNeteaseRTData(CNeteaseRTWebInquiry* pNeteaseRTWebData); // ���̷߳���ֵΪ3������ΪpNeteaseWebRTData
// ��ȡ����������ʷ�����߳�, pParamΪ���ô��̵߳ı�����ָ�롣��ַ��http://quotes.money.163.com/service/chddata.html?code=
UINT ThreadReadNeteaseDayLine(CNeteaseDayLineWebInquiry* pNeteaseDayLineWebData);      // �˹����̷߳���ֵΪ4������ΪpNeteaseWebDayLineData
// ��ȡcrweber.com��վ�ϵ�����ָ���̡߳���ַ��http://www.crweber.com
UINT ThreadReadCrweberIndex(CCrweberIndexWebInquiry* pCrweberIndexWebData); // �˹����̷߳���ֵΪ5������ΪpCrweberIndexWebData
// ��ȡPoten Daily Briefing.��ַ��http://energy.poten.com/poten-daily-briefing-webpage-04/11/2018
UINT ThreadReadPotenDailyBriefing(CPotenDailyBriefingWebInquiry* pPotenDailyBriefingWebData); // �˹����̷߳���ֵΪ6.
// ���ݿ⴦������

// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDayִ�о��������������8�������̡߳�
UINT ThreadCalculateDayLineRS(CChinaMarket* pMarket, long startCalculatingDay); // �˹����̷߳���ֵΪ11, ����Ϊ��ǰ����������
// ����������ڹ�Ʊ�������ǿ���̡߳����߳���ThreadCalculateRelativeStrong�̵߳��ã����ڲ������㲻ͬ���ڵ��������ǿ�ȣ������Ч��
UINT ThreadCalculateThisDayRS(CChinaMarket* pMarket, long thisDay); // �˹����̷߳���ֵҲΪ12
// �洢��ʱϵͳ״̬�߳�
UINT ThreadSaveTempRTData(CChinaMarket* pMarket);     // �˹����̷߳���ֵΪ13
// ����ʹ洢��ǰ�����������߳�
UINT ThreadProcessTodayStock(CChinaMarket* pMarket);     // �˹����̷߳���ֵΪ14
// �洢һ����Ʊ��������ʷ���ݡ�
UINT ThreadSaveDayLineOfOneStock(CChinaStockPtr pStock); // �˹����̷߳���ֵΪ15, ����ΪЯ������ָ���һ���ṹָ��
// �����ݿ��ж�ȡ������ʷ�����߳�
UINT ThreadLoadDayLine(CChinaStockPtr pStock);        // �˹����̷߳���ֵΪ16
// ά���������ݿ��̣߳���δʵ�֣�
UINT ThreadMaintainDayLineDataBase(void);   // �˹����̷߳���ֵΪ17
//���¹�Ʊ�������ݿ��߳�
UINT ThreadUpdateStockCodeDB(CChinaMarket* pMarket); // ���̷߳���ֵΪ18
//�洢��ѡ��Ʊʵʱ�����߳�
UINT ThreadSaveRTData(CChinaMarket* pMarket); // ���̷߳���ֵΪ19
// ����ѡ�����ݿ�
UINT ThreadUpdateOptionDB(CChinaMarket* pMarket); // ���̷߳���ֵΪ20
// �洢Poten.com�µ�����
UINT ThreadSavePotenData(CPotenDailyBriefingMarket* pMarket); // ���̷߳���ֵΪ21
// ����ѡ�����ݿ�
UINT ThreadAppendChoicedStockDB(CChinaMarket* pMarket); // ���̷߳���ֵΪ22
