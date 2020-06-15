#pragma once

enum {
  gl_ctGuadanAdd = 1,
  gl_ctGuadanDeal = 2,
  gl_ctGuadanWithdraw = 3,
};

class COneGuadan {
public:
  COneGuadan(void) {
    m_time = 0;
    m_lVolume = 0;
    m_lStatus = 0;
  }

  void SetTime(time_t t) { m_time = t; }
  time_t GetTime(void) { return m_time; }
  void SetVolume(long lVolume) { m_lVolume = lVolume; }
  long GetVolume(void) { return m_lVolume; }
  void SetStatus(long lStatus) { m_lStatus = lStatus; }
  long GetStatus(void) { return m_lStatus; }

  time_t			m_time;
  long				m_lVolume;
  long				m_lStatus;
};

class CGuadan {
public:
  CGuadan(void);
  ~CGuadan(void);

  // ÒÑ²âÊÔº¯Êý
  void				AddGuadan(COneGuadan oneGuadan);

  COneGuadan& GetGuadan(void);
  COneGuadan& GetGuadan(long lIndex);

  long				GetCurrentPos(void) { return m_lCurrentPos; }
  long				GetCompletedPos(void) { return m_lCompletedPos; }

  long				GetCurrentVolume(void) { return m_lCurrentVolume; }
  long				GetFormerVolume(void) { return m_lFormerVolume; }
  void				SetCurrentVolume(long lVolume);

  // ´ý²âÊÔº¯Êý
  void				ReSet(void);
  void				SetStatus(long lPos, long lStatus);
  //	void				ProcessGuadan(

protected:
  long					m_lCurrentVolume;		//
  long					m_lFormerVolume;		//
  CArray<COneGuadan, COneGuadan&> m_aGuadan;
  long					m_lCurrentPos;
  long					m_lCompletedPos;
};
