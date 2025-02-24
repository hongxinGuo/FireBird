#include "pch.h"

#include "SaveAndLoad.h"

void SaveToFile(const CString& strFileName, CString s) {
	CFile f;
	if (f.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) {
		f.Write(s.GetBuffer(), s.GetLength());
		f.Close();
	}
}

bool LoadFromFile(const CString& strFileName, string& s) {
	char buffer[1025]{};
	int iRead;
	CFile f(strFileName, CFile::modeRead | CFile::typeBinary);
	int i = 0;
	do {
		iRead = f.Read(buffer, 1024);
		buffer[iRead] = 0x000;
		s += buffer;
		i++;
	} while (iRead == 1024);
	f.Close();
	return true;
}
