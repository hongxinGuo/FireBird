#pragma once

class CRSReference {
public:
	bool m_fActive{ false };
	long m_lDayLength[4]{ 0, 0, 0, 0 }; // ���ڳ���
	long m_lStrongDayLength[4]{ 0, 0, 0, 0 }; // ��������������
	double m_dRSStrong[4]{ 50.0, 50.0, 50.0, 50.0 }; // ��ֵ��˼Ϊ���ڴ�ֵ����ֵ��˼Ϊ���ڴ�ֵ
};
