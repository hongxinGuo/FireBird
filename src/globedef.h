#pragma once

#ifndef __AFXWIN_H__
#error"�ڰ������ļ�֮ǰ�����Ȱ�����stdafx.h���ļ������� PCH �ļ�"
#endif

#if _MSVC_LANG <= 201703
#error "��ϵͳʹ��C++20�����ϱ�׼"
#endif

#include"SystemConstant.h"
#include"SystemConfiguration.h"
#include"SystemStatus.h"
#include"SystemData.h"
#include"SystemMessage.h"

#include"ClassDeclaration.h"

#include"HighPerformanceCounter.h"
extern CHighPerformanceCounter gl_counter;
extern bool gl_bGlobeVariableInitialized;
extern time_t gl_tUTC; // ���е��г�ʹ��ͬһ��Э������ʱ��Coordinated Universal Time��
