#pragma once

#include"stdafx.h"

#include"globedef.h"

// ʱ��任����buffer�е��ַ�������strFormat����ʽ�任��time_t��ʽ������ʱ��
time_t ConvertBufferToTime(CString strFormat, char* buffer);

// ���ڸ����ݼ���GetDefaultConnect()
CString GetDefaultSchemaConnect();

