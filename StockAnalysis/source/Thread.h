#pragma once

#include"stdafx.h"

// �������߳�����
// �߳��޷���Ϊ�ຯ���������У��޷���afxBeginThread()���ã����ʶ����̶߳�ֻ�ܲ���ȫ�ֺ�����ʽ���ڴ˴���
// �����߳����ڴ����ʱ����Ϊ�˼򻯳��������Ŀǰֻ������������Ϊ�̣߳�һ���Ǵ������ݿ��������һ����Զ���첽�����������ݵ�����
// �����������������̣߳��Լ���ͬ������ķ�������

// �����ȡ����

// ��ȡ����ʵʱ�����̡߳���ַ��http://hq.sinajs.cn/list=
UINT ThreadReadSinaRTData(LPVOID pParam);    // �˹����̷߳���ֵΪ1

// ��ȡ��Ѷʵʱ�����̡߳���ַ��http://qt.gtimg.cn/q=
UINT ThreadReadTengxunRTData(LPVOID pParam);    // �˹����̷߳���ֵΪ10

// ��ȡ����ʵʱ�����̣߳���ַ��http://api.money.126.net/data/feed/
UINT ThreadReadNeteaseRTData(LPVOID pParam); // ���̷߳���ֵΪ13

// ��ȡ����������ʷ�����߳�, pParamΪ���ô��̵߳ı�����ָ�롣��ַ��http://quotes.money.163.com/service/chddata.html?code=
UINT ThreadReadNeteaseDayLine(LPVOID pParam);      // �˹����̷߳���ֵΪ2

// ��ȡcrweber.com��վ�ϵ�����ָ���̡߳���ַ��www.crweber.com
UINT ThreadReadCrweberIndex(LPVOID pParam); // �˹����̷߳���ֵΪ12

// ����ֵΪ3���̱߳�ɾ��

// ���ݿ⴦������

// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDayִ�о��������������8�������̡߳�
UINT ThreadCalculateRS(LPVOID pParam); // �˹����̷߳���ֵΪ8

// ����������ڹ�Ʊ�������ǿ���̡߳����߳���ThreadCalculateRelativeStrong�̵߳��ã����ڲ������㲻ͬ���ڵ��������ǿ�ȣ������Ч��
UINT ThreadCalculateThisDayRS(LPVOID pParam); // �˹����̷߳���ֵΪ9
UINT ThreadCalculateThisDayRSUsingSemaphore(LPVOID pParam); // �˹����̷߳���ֵҲΪ9

// �洢��ʱϵͳ״̬�߳�
UINT ThreadSaveTempRTData(LPVOID pParam);     // �˹����̷߳���ֵΪ4

// ����ʹ洢��ǰ�����������߳�
UINT ThreadCompileCurrentTradeDayStock(LPVOID pParam);     // �˹����̷߳���ֵΪ5

// �洢һ����Ʊ��������ʷ���ݡ�
UINT ThreadSaveDayLineOfOneStock(LPVOID pParam); // �˹����̷߳���ֵΪ6

// �����ݿ��ж�ȡ������ʷ�����߳�
UINT ThreadLoadDayLine(LPVOID pParam);        // �˹����̷߳���ֵΪ7

// ά���������ݿ��̣߳���δʵ�֣�
UINT ThreadMaintainDayLineDataBase(LPVOID pParam);   // �˹����̷߳���ֵΪ11

//���¹�Ʊ�������ݿ��߳�
UINT ThreadUpdateStockCodeDB(LPVOID pParam); // ���̷߳���ֵΪ14