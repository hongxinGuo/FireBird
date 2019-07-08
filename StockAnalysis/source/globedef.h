#ifndef __GLOBE_DEFINE__
#define __GLOBE_DEFINE__

#include"stdafx.h"

#include"RTData.h"
#include"SetDayLine.h"
#include"StockID.h"

#include"SystemMessage.h"
#include"SystemDequeData.h"
#include"SystemStatus.h"

using namespace std;
#include<deque>
#include<vector>
#include<map>
#include<deque>
#include<memory>

// �������������ʹ�ö��ֽ��ַ�������unicode�ַ�����Ŀǰ��Ϊʹ�ö��ֽ��ַ�������Ϊgoogletest�޷�ʹ��unicode��
#define __USING_UNICODE_CHARSET__
//#define __USING_ANSI_CHARSET__

class CMarket;

class CStock;

extern CString gl_sCurrentBaseDir;
extern CString gl_sDataBaseDir;
extern CString gl_sDataBaseName;

extern CMarket gl_ChinaStockMarket;        // �г������л�Ծ�Ĺ�Ʊ��λ�����У���һʵ�����������������һ��ʵ����


extern CString gl_strRTStockSource;
extern DWORD	 gl_dwRTDataHttpFileStatus;

extern CString gl_strDayLineStockSource;
extern CString gl_strDayLinePostfix;
extern DWORD	 gl_dwDayLineHttpFileStatus;

extern CString	gl_strCurrentStockDownLoading; // Ŀǰ��������������ʷ���ݵĹ�Ʊ����

extern vector<StockIDPtr>	gl_vTotalStock;             // ��ϵͳ��������й�Ʊ�أ����۴����Ƿ���ڣ�
extern map<CString, long>	gl_mapTotalStockToIndex;		// �����б���ѯ�Ĺ�Ʊ����ӳ��Ϊƫ������Ŀǰֻ����A����Ϣ��

extern vector<StockIDPtr> gl_vStockChoice;   // ��ѡ��Ʊ��

extern long gl_lToday;          // �������ڡ���ʽΪ��19990102
extern long gl_lLastTradeDay; // ��һ�������գ������ж��Ƿ������������ݡ��������ݵ������գ�������һ�������գ��������յ�������ʵʱ���ݲ�����
extern long gl_lTime;				  // ����ʱ�䡣��ʽΪ��hhmmss
extern tm gl_tm;						  // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������
extern time_t gl_ttime;       // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������

extern long gl_lRelativeStrongStartDay;
extern long gl_lRelativeStrongEndDay;

extern CSetDayLine gl_setSavingDayLineOnly; // �˱���ר���ڴ洢���յ���������ʷ���ݡ�

extern CSystemMessage gl_systemMessage;       // ϵͳ��Ϣ�����࣬�����������߳���ʹ��
extern CSystemDequeData gl_systemDequeData;    // ϵͳ�еĸ��ֶ��У������������߳�ʹ�á�
extern CSystemStatus gl_systemStatus;         // ϵͳ�еĸ���״̬�������������߳���ʹ�á�

extern deque<CString> gl_dequeRTStockInquire; // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ����
extern deque<CString> gl_dequeDayLineStockInquire; // ����������ʷ���ݵĹ�Ʊ���С�

typedef struct stDayLine {    // �������ݽṹ
  stDayLine() {
    lByteRead = 0;
    fError = false;
    strInquire = "";
  }


  CString strInquire;   // ������ȡ���ݵ��ַ���
  char buffer[2048 * 1024]; // ��ȡ����������ʷ���ݻ�����
  long lByteRead;           // ���ζ������ֽ���
  bool fError;              // ���δ������������Ƿ�����ı�ʶ

protected:


} DayLineInquire;

extern DayLineInquire gl_stDayLineInquire; 

typedef struct stRTData {
  stRTData() {
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

extern bool gl_fExiting;

extern long gl_lOption_CalculatingRelativeStrongStartDay;
extern bool gl_fExitingCalculatingRelativeStrong;
#endif
