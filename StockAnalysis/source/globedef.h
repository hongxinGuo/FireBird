#ifndef __GLOBE_DEFINE__
#define __GLOBE_DEFINE__

#include"stdafx.h"

#include"RTData.h"

#include"SetDayLine.h"
#include"StockID.h"

#include"SinaRTWebData.h"
#include"TengxunRTWebData.h"
#include"NeteaseDayLineWebData.h"
#include"CrweberIndexWebData.h"

#include"CrweberIndex.h"

#include"SystemMessage.h"
#include"QueueRTData.h"
#include"ThreadStatus.h"
#include"SystemTime.h"

#include"CriticalSectionBool.h"

using namespace std;
#include<deque>
#include<vector>
#include<map>
#include<deque>
#include<memory>

extern CSinaRTWebData     gl_SinaRTWebData;     // ����ʵʱ���ݲɼ�
extern CTengxunRTWebData  gl_TengxunRTWebData;  // ��Ѷʵʱ���ݲɼ�
extern CNeteaseDayLineWebData gl_NeteaseDayLineWebData; // ����������ʷ����
extern CCrweberIndexWebData gl_CrweberIndexWebData;   // crweber.com�ϵ�ÿ������ָ��

extern CCrweberIndex gl_CrweberIndex;

extern CSystemMessage     gl_systemMessage;     // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����
extern CSystemTime        gl_systemTime;        // ϵͳʱ����ܡ�
extern CQueueRTData       gl_QueueRTData;       // ϵͳʵʱ���ݶ��С�
extern CThreadStatus      gl_ThreadStatus;      // ϵͳ�еĸ���״̬�������������߳���ʹ��

extern deque<CString> gl_dequeRTStockInquire;   // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ����
extern deque<CString> gl_dequeDayLineStockInquire; // ����������ʷ���ݵĹ�Ʊ���С�

extern CCriticalSectionBool gl_ExitingSystem; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
extern bool gl_fExitingCalculatingRS;           // ����֪ͨ�����߳��˳����ź�
extern bool gl_fResetSystem;                    // ϵͳ������ʶ

extern bool gl_fNormalMode;                     // ����ģʽ��ʶ��Ĭ��Ϊ�١�ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
extern bool gl_fTestMode;                       // �Ƿ�������gl_fTestMode��ʶ�������ж��Ƿ���ʵ��ϵͳ��ʹ����MySQLTest��������

class CMarket;
// ���г���������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�
extern CMarket gl_ChinaStockMarket;        // �г������л�Ծ�Ĺ�Ʊ��λ�����У���һʵ�����������������һ��ʵ����

#endif
