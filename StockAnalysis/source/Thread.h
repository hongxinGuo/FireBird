#pragma once

#include"stdafx.h"

// �������߳�����
// �߳��޷���Ϊ�ຯ���������У��޷���afxBeginThread()���ã����ʶ����̶߳�ֻ�ܲ���ȫ�ֺ�����ʽ���ڴ˴���

// ��ȡ����ʵʱ�����̡߳�
UINT ThreadReadSinaRTData(LPVOID pParam);    // �˹����̷߳���ֵΪ1

// ��ȡ����������ʷ�����߳�, pParamΪ���ô��̵߳ı�����ָ��
UINT ThreadReadNeteaseDayLine(LPVOID pParam);      // �˹����̷߳���ֵΪ2

// ����ʵʱ�����߳�
UINT ThreadCalculateRTData(LPVOID pParam);  // �˹����̷߳���ֵΪ3

// �洢��ʱϵͳ״̬�߳�
UINT ThreadSaveTempRTData(LPVOID pParam);     // �˹����̷߳���ֵΪ4

// ����ʹ洢��ǰ�����������߳�
UINT ThreadCompileCurrentTradeDayStock(LPVOID pParam);     // �˹����̷߳���ֵΪ5

// �洢һ����Ʊ��������ʷ���ݡ����̱߳�ThreadSaveDayLine�̼߳�ӵ���.��δʵ��
UINT ThreadSaveDayLineOfOneStock(LPVOID pParam); // �˹����̷߳���ֵΪ6

// �����ݿ��ж�ȡ������ʷ�����߳�
UINT ThreadLoadDayLine(LPVOID pParam);        // �˹����̷߳���ֵΪ7

// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDayִ�о��������������8�������̡߳�
UINT ThreadCalculateRS(LPVOID pParam); // �˹����̷߳���ֵΪ8

// �������ݿ��̣߳���δʵ�֣�
UINT ThreadUpdateDataBase(LPVOID pParam);   // �˹����̷߳���ֵΪ9

// ��ȡ��Ѷʵʱ�����̡߳�
UINT ThreadReadTengxunRTData(LPVOID pParam);    // �˹����̷߳���ֵΪ10

// ����������ڹ�Ʊ�������ǿ���̡߳����߳���ThreadCalculateRelativeStrongProcx�̵߳��ã����ڲ������㲻ͬ���ڵ��������ǿ�ȣ������Ч��
UINT ThreadCalculateThisDayRS(LPVOID pParam); // �˹����̷߳���ֵΪ11

// ��ȡcrweber.com��վ�ϵ�����ָ���߳�
UINT ThreadReadCrweberIndex(LPVOID pParam); // �˹����̷߳���ֵΪ12
