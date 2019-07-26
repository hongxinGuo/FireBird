#ifndef __GLOBE_DEFINE__
#define __GLOBE_DEFINE__

#include"stdafx.h"

#include"RTData.h"

#include"SetDayLine.h"
#include"StockID.h"

#include"SystemMessage.h"
#include"SystemDequeData.h"
#include"SystemStatus.h"
#include"SystemTime.h"

using namespace std;
#include<deque>
#include<vector>
#include<map>
#include<deque>
#include<memory>

class CMarket;

extern CSystemMessage     gl_systemMessage;     // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����
extern CSystemTime        gl_systemTime;        // ϵͳʱ����ܡ�
extern CSystemDequeData   gl_systemDequeData;   // ϵͳ�еĸ��ֶ��У������������߳�ʹ�á�
extern CSystemStatus      gl_systemStatus;      // ϵͳ�еĸ���״̬�������������߳���ʹ�á�

extern vector<StockIDPtr> gl_vStockChoice;      // ��ѡ��Ʊ��

extern CSetDayLine gl_setSavingDayLineOnly;     // �˱���ר���ڴ洢���յ���������ʷ���ݡ���Զ��״̬������ͬʱ�������ݿ�ʱ�ٶ�������

extern deque<CString> gl_dequeRTStockInquire;   // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ����
extern deque<CString> gl_dequeDayLineStockInquire; // ����������ʷ���ݵĹ�Ʊ���С�

typedef struct stDayLine {    // �������ݽṹ
  stDayLine() noexcept {
    lByteRead = 0;
    fError = false;
    strInquire = "";
  }

  CString strInquire;       // ������ȡ���ݵ��ַ���
  char buffer[2048 * 1024]; // ��ȡ����������ʷ���ݻ�����
  long lByteRead;           // ���ζ������ֽ���
  bool fError;              // ���δ������������Ƿ�����ı�ʶ

protected:

} DayLineInquire;

extern DayLineInquire gl_stDayLineInquire; 

typedef struct stRTData {
  stRTData() noexcept {
    lByteRead = 0;
    fError = false;
    strInquire = "";
  }
  CString strInquire;
  char buffer[2048 * 1024];
  long lByteRead;
  bool fError;
} RTDataInquire;

extern RTDataInquire gl_stRTDataInquire;

extern bool gl_fExiting; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
extern bool gl_fExitingCalculatingRelativeStrong;// ����֪ͨ�����߳��˳����ź�
extern bool gl_fResetSystem; // ϵͳ������ʶ

extern long gl_lShowGuaDanTransaction; // ��ʾ�ҵ��ɽ��������

// ���г���������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�
extern CMarket gl_ChinaStockMarket;        // �г������л�Ծ�Ĺ�Ʊ��λ�����У���һʵ�����������������һ��ʵ����

extern clock_t gl_RTReadingTime;          // ÿ�ζ�ȡ����ʵʱ���ݵ�ʱ��
extern clock_t gl_DayLineReadingTime;     // ÿ�ζ�ȡ����������ʷ���ݵ�ʱ��


#endif
