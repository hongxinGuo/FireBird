//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���е�ȫ�ֲ�����λ�ڴ��ļ��У�����ϵͳ���á�
//
// �洢��ʽʹ��json��ʽ���洢���ļ�systemOption.json�С�
//
// ʹ��boost�е�JSON��������֮��ʵ�����л��ͷ����л���
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "nlohmann/json.hpp"
using nlohmann::json;

class CGlobeOption : CObject {
public:
	CGlobeOption();
	~CGlobeOption();

	bool Load();
	bool Save();

	bool AssignKey(const char* key, CString value);
	bool AssignKey(const char* key, int value);
	bool AssignKey(const char* key, bool value);
	bool AssignKey(const char* key, double value);
	bool NewKey(const char* key, CString value);
	bool NewKey(const char* key, int value);
	bool NewKey(const char* key, bool value);
	bool NewKey(const char* key, double value);

	CString GetString(const char* key) { return (CString)m_systemOption[key]; }
	int GetInt(const char* key) { return (int)m_systemOption[key]; }
	bool GetBool(const char* key) { return (bool)m_systemOption[key]; }
	double GetDouble(const char* key) { return (double)m_systemOption[key]; }

public:

protected:
	json m_systemOption;
	bool m_fUpdate;
	CString m_strFileName;
};

extern CGlobeOption gl_GlobeOption;
