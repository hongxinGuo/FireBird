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

#include"ThreadStatus.h"
#include"ChinaStock.h"

UINT ThreadSaveDayLineBasicInfo(not_null<CChinaStock*> pStock) {
	gl_SaveDayLineThreadPermitted.acquire(); //ʹ�ö��߳�ģʽ��
	gl_ThreadStatus.IncreaseSavingThread();
	if (!gl_systemStatus.IsExitingSystem()) {
		const bool fDataSaved = pStock->SaveDayLineBasicInfo();
		pStock->UpdateDayLineStartEndDate();
		if (fDataSaved) {
			const CString str = pStock->GetSymbol() + _T("�������ϴ洢���");
			gl_systemMessage.PushDayLineInfoMessage(str);
		}
		pStock->UnloadDayLine();
	}
	gl_ThreadStatus.DecreaseSavingThread();
	gl_SaveDayLineThreadPermitted.release();

	return 15;
}
