#pragma once
#include "CWebData.h"
#include"thread.h"

class CSinaRTWebData : public CWebData {
  CSinaRTWebData() :CWebData() { }
  ~CSinaRTWebData() {}

  //virtual void ProcessWebData(void);
  virtual bool ReadAndSaveWebData(char*& pCurrentPos, long& iCount);
  virtual void ReportDataError(void);
  virtual void ReportCommunicationError(void);
  virtual void InquireNextWebData(void);
  virtual int GetInquiringStockStr(CString& strInquire);
  virtual void StartReadingThread(void) { AfxBeginThread(ThreadReadSinaRTData, nullptr); }
  virtual bool ProcessRTDataReveived(void);
};
