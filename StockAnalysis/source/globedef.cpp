#include"globedef.h"

#include"StockID.h"
#include "Market.h"
#include "SystemTime.h"

CSystemTime gl_sTime;

vector<StockIDPtr>		gl_vTotalStock;
map<CString, long>		gl_mapTotalStockToIndex;		// �����б���ѯ�Ĺ�Ʊ����ӳ��Ϊƫ������Ŀǰֻ����A����Ϣ��

vector<StockIDPtr> gl_vStockChoice;   // ��ѡ��Ʊ��

CSetDayLine gl_setSavingDayLineOnly; // �˱���ר���ڴ洢���յ���������ʷ���ݡ���Զ��״̬������ͬʱ�������ݿ�ʱ�ٶ�������

CSystemTime       gl_systemTime;            // ϵͳʱ����ܡ�
CSystemMessage    gl_systemMessage;         // ϵͳ��Ϣ������
CSystemDequeData  gl_systemDequeData;       // ϵͳ�еĸ��ֶ��У������������߳�ʹ�á�
CSystemStatus     gl_systemStatus;          // ϵͳ�еĸ���״̬�������������߳���ʹ�á�

deque<CString> gl_dequeRTStockInquire; // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ����
deque<CString> gl_dequeDayLineStockInquire; // ����������ʷ���ݵĹ�Ʊ���С�

DayLineInquire gl_stDayLineInquire;
RTDataInquire gl_stRTDataInquire;

bool gl_fExiting = false; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
bool gl_fExitingCalculatingRelativeStrong = false;

long gl_lShowGuaDanTransaction = __ORDINARY_BUY__; // ��ʾ�ҵ��ɽ��������

CMarket gl_ChinaStockMarket; // ���г�ʵ������λ��ȫ�ֱ����������Ϊ���ʼ����Ҫ����ȫ�ֱ�����֧�֡�

