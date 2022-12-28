#pragma once

class CSystemStatus final : public CObject {
public:
	CSystemStatus();
	// ֻ����һ��ʵ��,������ֵ��
	CSystemStatus(const CSystemStatus&) = delete;
	CSystemStatus& operator=(const CSystemStatus&) = delete;
	CSystemStatus(const CSystemStatus&&) noexcept = delete;
	CSystemStatus& operator=(const CSystemStatus&&) noexcept = delete;
	~CSystemStatus() override = default;

	void SetExitingSystem(const bool bExit) { m_fExitingSystem = bExit; }
	bool IsExitingSystem() { return m_fExitingSystem; }
	void SetExitingCalculatingRS(const bool bExit) { m_fExitingCalculatingRS = bExit; }
	bool IsExitingCalculatingRS() const { return m_fExitingCalculatingRS; }
	void SetWorkingMode(const bool bNormal) { m_fWorkingMode = bNormal; }
	bool IsWorkingMode() const { return m_fWorkingMode; }

protected:
	std::atomic_bool m_fExitingSystem; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
	bool m_fExitingCalculatingRS; // ����֪ͨ�����߳��˳����ź�
	bool m_fWorkingMode; // ����ģʽ��ʶ��Ĭ��Ϊ�١�ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
};

extern CSystemStatus gl_systemStatus;
