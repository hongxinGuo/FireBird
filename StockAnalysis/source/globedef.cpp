#include"globedef.h"

#include"StockID.h"
#include "Stock.h"
//#include "RTData.h"
#include "Market.h"
#include "CurrentTime.h"
#include "MutualLock.h"

CMarket gl_ChinaStockMarket;

CCurrentTime gl_sTime;

CMutualLock gl_sMutualLock;

vector<StockIDPtr>		gl_vTotalStock;
map<CString, long>		gl_mapTotalStockToIndex;		// �����б���ѯ�Ĺ�Ʊ����ӳ��Ϊƫ������Ŀǰֻ����A����Ϣ��

vector<StockIDPtr> gl_vStockChoice;   // ��ѡ��Ʊ��

CString		gl_strCurrentStockDownLoading;

long gl_lToday;           // �������ڡ���ʽΪ��19990102
long gl_lLastTradeDay;  // ��һ�������գ������ж��Ƿ������������ݡ��������ݵ������գ�������һ�������գ��������յ�������ʵʱ���ݲ�����
long gl_lTime;				  // ����ʱ�䡣��ʽΪ��hhmmss
tm gl_tm;						    // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������
time_t gl_ttime;        // ȫ��ʱ�䡣��CMainFrame��OnTimer�����������

long gl_lRelativeStrongStartDay;
long gl_lRelativeStrongEndDay;


CSetDayLine gl_setSavingDayLineOnly; // �˱���ר���ڴ洢���յ���������ʷ���ݡ�

CSystemMessage gl_systemMessage;       // ϵͳ��Ϣ������
CSystemDequeData gl_systemDequeData;    // ϵͳ�еĸ��ֶ��У������������߳�ʹ�á�
CSystemStatus gl_systemStatus;         // ϵͳ�еĸ���״̬�������������߳���ʹ�á�

deque<CString> gl_dequeRTStockInquire; // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ����
deque<CString> gl_dequeDayLineStockInquire; // ����������ʷ���ݵĹ�Ʊ���С�

DayLineInquire gl_stDayLineInquire;
RTDataInquire gl_stRTDataInquire;

bool gl_fExiting = false; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�



