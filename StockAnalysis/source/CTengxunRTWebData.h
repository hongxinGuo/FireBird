#pragma once
#include "CWebData.h"
#include"thread.h"

class CTengxunRTWebData : public CWebData {
  CTengxunRTWebData() : CWebData() {}
  ~CTengxunRTWebData() {}

  virtual void ProcessWebData(void);
  virtual void InquireNextWebData(void);
  virtual int GetInquiringStockStr(CString& strInquire);
  virtual void StartReadingThread(void) { AfxBeginThread(ThreadReadTengxunRTData, nullptr); }
  virtual bool ProcessRTDataReveived(void);
};
