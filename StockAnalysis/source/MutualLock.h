#pragma once

class CMutualLock : public CObject {
public:
	CMutualLock( void );

public:
	bool					WaitDayKLineReply() { return m_fWaitDayKLineReply; }
	void					SetWaitDayKLineReply( bool f ) { m_fWaitDayKLineReply = f; }
	
	bool					WaitPankouReply() { return m_fWaitPankouReply; }
	void					SetWaitPankouReply( bool f ) { m_fWaitPankouReply = f; }

	bool					IsShowShowStock( void ) { return( m_fShowShowStock ); }
	void					SetShowShowStock( bool f ) { m_fShowShowStock = f; }

	bool					IsShowUpdownStock( void ) { return( m_fShowUpdownStock ); }
	void					SetShowUpdownStock( bool f ) { m_fShowUpdownStock = f; }

	bool					IsShowSelfChoicedStock( void ) { return( m_fShowSelfChoicedStock ); }
	void					SetShowSelfChoicedStock( bool f ) { m_fShowSelfChoicedStock = f; }

	bool					IsDebugRTData( void ) { return( m_fDebugRTData ); }
	void					SetDebugRTData( bool f ) { m_fDebugRTData = f; }

protected:
	bool							m_fWaitDayKLineReply;	// �Ƿ�ȴ����������Ӧ
	bool							m_fWaitPankouReply;			// �Ƿ�ȴ��̿����������Ӧ

	bool							m_fShowUpdownStock;		// ��ʾ�ǵ�������
	bool							m_fShowSelfChoicedStock;	// ��ʾ��ѡ��Ʊ
	bool							m_fShowShowStock;						// ��ʾ��ǰѡ�еĹ�Ʊ

	bool							m_fDebugRTData;

};