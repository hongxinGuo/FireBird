#include"pch.h"

module FireBird.Type.Guadan;

CGuadan::CGuadan() {
	m_lCurrentVolume = 0;
	ReSet();
}

////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////
void CGuadan::ReSet() {
	m_lCurrentVolume = 0; //
	m_lFormerVolume = 0; //
	m_lMaxPos = 0;
	m_vGuadan.resize(0);
	m_lCompletedPos = 0;
}

void CGuadan::AddGuadan(COneGuadan oneGuadan) {
	m_vGuadan.push_back(oneGuadan);
	m_lMaxPos++;
}

COneGuadan& CGuadan::GetGuadan() {
	return (m_vGuadan.at(m_lMaxPos - 1));
}

COneGuadan& CGuadan::GetGuadan(long lIndex) {
	return (m_vGuadan.at(lIndex));
}

void CGuadan::SetCurrentVolume(long lVolume) {
	m_lFormerVolume = m_lCurrentVolume;
	m_lCurrentVolume = lVolume;
}

/////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
void CGuadan::SetStatus(long lPos, long lStatus) {
	m_vGuadan.at(lPos).SetStatus(lStatus);
}
