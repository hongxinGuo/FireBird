#pragma once


#include<atomic>

class CSystemStatus : public CObject {
public:
	CSystemStatus();

	virtual ~CSystemStatus() {
	}

	void SetExitingSystem(bool bExit) { m_fExitingSystem = bExit; }
	bool IsExitingSystem() { return m_fExitingSystem; }
	void SetExitingCalculatingRS(bool bExit) { m_fExitingCalculatingRS = bExit; }
	bool IsExitingCalculatingRS() { return m_fExitingCalculatingRS; }
	void SetWorkingMode(bool bNormal) { m_fWorkingMode = bNormal; }
	bool IsWorkingMode() { return m_fWorkingMode; }

protected:
	std::atomic_bool m_fExitingSystem; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
	bool m_fExitingCalculatingRS; // ����֪ͨ�����߳��˳����ź�
	bool m_fWorkingMode; // ����ģʽ��ʶ��Ĭ��Ϊ�١�ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
};

extern CSystemStatus gl_systemStatus;
