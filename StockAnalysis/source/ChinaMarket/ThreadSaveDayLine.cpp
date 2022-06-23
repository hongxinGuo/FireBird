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
#include"SemaphoreDef.h"
#include"ChinaStock.h"

UINT ThreadSaveDayLineBasicInfoOfStock(not_null<CChinaStock*> pStock) {
	CString str;
	bool fDataSaved = false;

	gl_ThreadStatus.IncreaseSavingThread();
	gl_SaveThreadPermitted.Wait(); //ʹ�ö��߳�ģʽ����������ȫ����ʷ����ʱʹ��4���̣߳�������ʷ����ʱֻʹ��һ���̣߳���ʱʹ�ö���̷߳��������ֻ�����󣩡�
	if (!gl_systemStatus.IsExitingSystem()) {
		fDataSaved = pStock->SaveDayLineBasicInfo();
		pStock->UpdateDayLineStartEndDate();
		if (fDataSaved) {
			str = pStock->GetSymbol() + _T("�������ϴ洢���");
			gl_systemMessage.PushDayLineInfoMessage(str);
		}
		pStock->UnloadDayLine();
	}
	gl_SaveThreadPermitted.Signal();
	gl_ThreadStatus.DecreaseSavingThread();

	return 15;
}