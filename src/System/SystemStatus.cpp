#include"pch.h"

#include "SystemStatus.h"

CSystemStatus::CSystemStatus() {
	ASSERT(gl_bStartInitializeGlobeVariable);
	m_fExitingSystem = false;
	m_fExitingCalculatingRS = false;
	m_fWorkingMode = false;
}
