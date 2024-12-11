#include"pch.h"
export module FireBird.Accessory.SaveAndLoad;

export void SaveToFile(const CString& strFileName, CString s);
export bool LoadFromFile(const CString& strFileName, string& s);
