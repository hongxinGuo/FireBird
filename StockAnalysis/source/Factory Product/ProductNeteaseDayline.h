#pragma once

using namespace std;
#include<memory>
#include<string>

#include"ClassDeclaration.h"
#include"WebRTData.h"
#include"WebData.h"

#include"VirtualProductWebData.h"

class CProductNeteaseDayline : public CVirtualProductWebData {
public:
	DECLARE_DYNCREATE(CProductNeteaseDayline)
		CProductNeteaseDayline();
	~CProductNeteaseDayline() {}

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;

public:
	bool ParseNeteaseDayline(vector<CWebRTDataPtr>&, CWebDataPtr pWebData);

protected:
	long m_lCurrentStockPosition; // ��Ʊ��ǰ��ѯλ��
};