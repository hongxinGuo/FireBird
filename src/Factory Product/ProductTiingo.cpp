#include"pch.h"

#include"ProductTiingo.h"

bool CProductTiingo::IsValidData(const CWebDataPtr& pWebData) {
	if (pWebData->IsVoidJson()) return false;

	return true;
}
