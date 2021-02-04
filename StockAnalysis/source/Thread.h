#pragma once

#include"stdafx.h"

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"CrweberIndexWebInquiry.h"
#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

#include"ChinaStock.h"
class CChinaMarket;
#include"WorldMarket.h"
#include"WorldStock.h"
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
// ��ȡFinnhub.io���������̡߳� ��ַ��https://finnhub.io/api/v1/
UINT ThreadReadFinnhubData(not_null<CFinnhubWebInquiry*> pFinnhubWebInquiry); // �˹����̷߳���ֵΪ7��
// ��ȡapi.tiingo.com
UINT ThreadReadTiingoData(not_null<CTiingoWebInquiry*> pTiingoWebData); // �˹����̵߳ķ���ֵΪ8
// ��ȡquandl
UINT ThreadReadQuandlData(not_null<CQuandlWebInquiry*> pQuandlWebData); // �˹����̵߳ķ���ֵΪ9

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
UINT ThreadSaveDayLineBasicInfoOfStock(not_null<CChinaStock*> pStake); // �˹����̷߳���ֵΪ15, ����ΪЯ������ָ���һ���ṹָ��
// �����ݿ��ж�ȡ������ʷ�����߳�
UINT ThreadLoadDayLine(not_null<CChinaStock*> pStake);        // �˹����̷߳���ֵΪ16
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
UINT ThreadBuildWeekLineOfStock(not_null<CChinaStock*> pStake, long lStartDate); /// ���̵߳ķ���ֵΪ26
// �����ݿ��ȡ������ʷ����
UINT ThreadLoadWeekLine(not_null<CChinaStock*> pStake); // ���̷߳���ֵΪ29
// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDateִ�о��������������8�������̡߳�
UINT ThreadBuildWeekLineRS(not_null<CChinaMarket*> pMarket, long startCalculatingDate); // �˹����̷߳���ֵΪ30, ����Ϊ��ǰ����������
// ����������ڹ�Ʊ�������ǿ���̡߳����߳���ThreadCalculateRS�̵߳��ã����ڲ������㲻ͬ���ڵ��������ǿ�ȣ������Ч��
UINT ThreadBuildWeekLineRSOfDate(not_null<CChinaMarket*> pMarket, long lDate); // �˹����̷߳���ֵҲΪ31
// ���㱾�ܵ�����
UINT ThreadBuildWeekLineOfCurrentWeek(not_null<CChinaMarket*> pMarket); // �˹����̷߳���ֵΪ32
// �ؽ���ǰ�ܵ��������ݱ�
UINT ThreadBuildCurrentWeekWeekLineTable(not_null<CChinaMarket*> pMarket); // �˹����̷߳���ֵΪ33
// �洢֤ȯ���������
UINT ThreadSaveStakeSection(not_null<CChinaMarket*> pMarket); // �˹����̷߳���ֵΪ35

// ����Ϊ������Ʊ�г��Ĺ����߳�
// ��������������ֹ�����߳�
UINT ThreadUpdateStakeDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ37
// ����Forex��������
UINT ThreadUpdateForexDayLineDB(not_null<CFinnhubForexSymbol*> pSymbol); // �˹����̵߳ķ���ֵδ38
// ����Forex�����
UINT ThreadUpdateForexSymbolDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ39
// ����CountryList�����
UINT ThreadUpdateCountryListDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ40
// ����EPSSurprise��
UINT ThreadUpdateEPSSurpriseDB(not_null<CWorldStock*> pStake); // �˹����̷߳���ֵΪ41
// ����������Ʊ��������ʷ���ݡ�
UINT ThreadUpdateWorldStockDayLineDB(not_null<CWorldMarket*> pMarket); // �˹����̷߳���ֵΪ42
// �������������ؽ���ֹ����
UINT ThreadUpdateWorldStockDayLineStartEndDate(CWorldMarket* pMarket); // �˹����̷߳���ֵΪ43

//���ּ����ù����߳�
// ����10��ǿ��Ʊ������������ֻ����һ��ǿ�Ʒ壩
UINT ThreadChoice10RSStrong1StockSet(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ101
// ����10��ǿ��Ʊ�������γ���ǿ�Ʒ壩
UINT ThreadChoice10RSStrong2StockSet(not_null<CChinaMarket*> pMarket); // ���̷߳���ֵΪ102
// ����10��ǿ��Ʊ����ʹ���ⲿpRef�ṩ�Ĳ�����
UINT ThreadChoice10RSStrongStockSet(not_null<CChinaMarket*> pMarket, CRSReference* pRef, int iIndex); // ���̷߳���ֵΪ103
// �����Ʊ��10��ǿ�����
UINT ThreadCalculate10RSStrongStock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, CRSReference* pRef, not_null<CChinaStockPtr> pStake); // ���̷߳���ֵΪ104
// �����Ʊ��10��ǿ�����1
UINT ThreadCalculate10RSStrong1Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, not_null<CChinaStockPtr> pStake); // ���̷߳���ֵΪ105
// �����Ʊ��10��ǿ�����2
UINT ThreadCalculate10RSStrong2Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, not_null<CChinaStockPtr> pStake); // ���̷߳���ֵΪ105
