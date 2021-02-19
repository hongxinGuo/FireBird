#pragma once

#include"stdafx.h"
#include"globeSymbolDef.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CVirtualStock : public CObject {
public:
  CVirtualStock();
  ~CVirtualStock();
  void Reset(void);

public:
  bool IsTodayNewStock(void) noexcept { return m_fTodayNewStock; }
  void SetTodayNewStock(bool fFlag) noexcept { m_fTodayNewStock = fFlag; }
  bool IsUpdateStockProfileDB(void) noexcept { return m_fUpdateStockProfileDB; }
  void SetUpdateStockProfileDB(bool fFlag) noexcept { m_fUpdateStockProfileDB = fFlag; }
  bool IsActive(void) noexcept { return m_fActive; }
  void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }

protected:
  bool m_fTodayNewStock; // �����·��ֵĹ�Ʊ
  bool m_fUpdateStockProfileDB; 
  bool m_fActive;	// �Ƿ����������ݶ��롣������ʵʱ���鴦����������������ʷ���ݴ����������á�

};

typedef shared_ptr<CVirtualStock> CVirtualStockPtr;
