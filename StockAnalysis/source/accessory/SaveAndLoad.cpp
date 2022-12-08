#include "pch.h"

#include "SaveAndLoad.h"

void SaveToFile(CString strFileName, CString s) {
	CFile f;
	if (f.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) {
		f.Write(s.GetBuffer(), s.GetLength());
		f.Close();
	}
}

bool LoadFromFile(CString strFileName, string& s) {
	char buffer[1025]{};
	int iRead = 0;
	CFile f(strFileName, CFile::modeRead | CFile::typeBinary);

	do {
		iRead = f.Read(buffer, 1024);
		buffer[iRead] = 0x000;
		s += buffer;
	}
	while (iRead == 1024);
	f.Close();
	return true;
}
