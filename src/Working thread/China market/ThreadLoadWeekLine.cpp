#include"pch.h"

#include"ChinaMarket.h"
#include"Thread.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����װ�ع����߳�
//
// �����ݿ���װ����Ӧ��Ʊ���������ݣ�Ȼ���������ǿ��
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadLoadWeekLine(not_null<CChinaStock*> pStock) {
	pStock->UnloadWeekLine();
	// װ����������
	pStock->LoadWeekLine();
	// ��������ǿ�ȣ���ָ�����ǿ��ΪĬ��ֵ��
	pStock->CalculateWeekLineRSIndex();
	pStock->SetWeekLineLoaded(true);

	return 29;
}