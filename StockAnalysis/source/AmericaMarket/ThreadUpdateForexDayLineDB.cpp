//////////////////////////////////////////////////////////////////////////////////////
//
// �洢������ʷ���ݿ�ʱ�����ƻ���ͬ�����⣬���µ����ݿ��д��ھ�����ʱ�����µ���;�ͻᷢ�����ݿ�ͬ�����Ļ��⣬
// �����Ҷ�MySQL���ݿⲻ̫��Ϥ���޷��ҵ�����������ʶ����������ź������ֱ����������ݿ�Ϊ�գ�ϵͳ����)ʱ,
// ʹ�ö��̷߳�ʽ�洢���ݣ��������ݿ��д��ھ�����ʱ��ʹ�õ�һ�̴߳洢��
// ���ַ�ʽ����Ӱ���ٶȣ���Ϊ��������ʱ���������Ǻ�С�ġ�
//
// ���յĽ������������Ҫ�о��������ͬ�����⡣
//
//
/////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"globedef.h"

#include"FinnhubForexSymbol.h"

UINT ThreadUpdateForexDayLineDB(not_null<CForexSymbol*> pSymbol) {
	CString str;

	gl_ThreadStatus.IncreaseSavingThread();
	gl_UpdateWorldMarketDB.Wait();
	pSymbol->UpdateDayLineDB();
	pSymbol->UnloadDayLine();
	str = pSymbol->GetSymbol() + _T("�������ϴ洢���");
	gl_systemMessage.PushDayLineInfoMessage(str);
	gl_UpdateWorldMarketDB.Signal();
	gl_ThreadStatus.DecreaseSavingThread();

	return 38;
}