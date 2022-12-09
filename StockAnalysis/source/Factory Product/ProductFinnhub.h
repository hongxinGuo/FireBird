#pragma once

#include<memory>
#include<string>

#include"ClassDeclaration.h"

#include"VirtualProductWebData.h"

class CProductFinnhub : public CVirtualProductWebData {
public:
	DECLARE_DYNCREATE(CProductFinnhub)

	CProductFinnhub() {
	}

	~CProductFinnhub() {
	}

	virtual bool CheckNoRightToAccess(CWebDataPtr pWebData) override final;
	virtual bool AddInaccessibleExchangeIfNeeded(void) override; // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�

protected:
};
