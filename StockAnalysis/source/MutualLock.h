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
	bool							m_fWaitDayKLineReply;	// 是否等待日线请求回应
	bool							m_fWaitPankouReply;			// 是否等待盘口数据请求回应

	bool							m_fShowUpdownStock;		// 显示涨跌幅排名
	bool							m_fShowSelfChoicedStock;	// 显示自选股票
	bool							m_fShowShowStock;						// 显示当前选中的股票

	bool							m_fDebugRTData;

};