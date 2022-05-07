#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ�����Ȱ�����stdafx.h���ļ������� PCH �ļ�"
#endif

// �й���Ʊ�г���ʼ��
constexpr auto __CHINA_MARKET_BEGIN_DATE__ = 19900101;

#include"SystemMessage.h"
#include"ThreadStatus.h"
#include"WebRTDataContainer.h"

using namespace std;
#include<deque>
#include<atomic>
#include<vector>

extern atomic_bool gl_fExitingSystem; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
extern bool gl_fExitingCalculatingRS; // ����֪ͨ�����߳��˳����ź�

extern bool gl_fNormalMode; // ����ģʽ��ʶ��Ĭ��Ϊ�١�ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�

extern CSystemMessage gl_systemMessage; // ϵͳ��Ϣ�����ࡣ�˱���������ڵ�һλ������ȫ�ֱ�����ʼ��ʱ�õ��˱�����
extern CThreadStatus gl_ThreadStatus; // ϵͳ�еĸ���״̬�������������߳���ʹ��

extern CWebRTDataContainer gl_WebRTDataContainer; // ����ʵʱ���ݴ洢����

extern long long gl_lFrequency; // Ƶ�ʼ��������߾��ȼ�����ÿ��Ƶ��

extern deque<CString> gl_dequeRTStockInquire; // ����ʵʱ��Ʊ��Ϣ�Ĺ�Ʊ���У���δʹ�ã�
extern deque<CString> gl_dequeDayLineStockInquire; // ����������ʷ���ݵĹ�Ʊ���У���δʹ�ã�
