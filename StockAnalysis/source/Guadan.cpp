#include "stdafx.h"

#include "Guadan.h"

CGuadan::CGuadan(void) {
  m_lCurrentVolume = 0;
	ReSet();
}

CGuadan::~CGuadan(void)
{
}

////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////
void CGuadan::ReSet( void ) {
	m_lCurrentVolume = 0;		// 
	m_lFormerVolume = 0;		//
	m_lCurrentPos = 0;
	m_lCompletedPos = 0;
	m_aGuadan.RemoveAll();
	//�󲿷ֵĹҵ���λ��û�б仯��������������������ʱ�仯�ͺܶ࣬����������λΪ10 �����Լ����ڴ��ƶ�������
	m_aGuadan.SetSize( 1, 10 );
}

void CGuadan::AddGuadan( COneGuadan oneGuadan ) {
	m_aGuadan.SetAtGrow( m_lCurrentPos++, oneGuadan );
}

COneGuadan& CGuadan::GetGuadan( void ) {
	return( m_aGuadan.GetAt( m_lCurrentPos - 1 ) );
}

COneGuadan& CGuadan::GetGuadan( long lIndex ) {
	ASSERT( (lIndex >= 0) && (lIndex < m_lCurrentPos) );
	return( m_aGuadan.GetAt( lIndex ) );
}

void CGuadan::SetCurrentVolume( long lVolume ) { 
	m_lFormerVolume = m_lCurrentVolume;
	m_lCurrentVolume = lVolume; 
}

/////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
void CGuadan::SetGuadanStatus( long lPos, long lStatus ) {
	ASSERT( (lPos >= 0) && (lPos < m_lCurrentPos) );
	m_aGuadan[lPos].SetStatus( lStatus );
}