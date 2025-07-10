#include "pch.h"

#include "SaveAndLoad.h"

void SaveToFile(const string& strFileName, string s) {
	CFile f;
	if (f.Open(strFileName.c_str(), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) {
		f.Write(s.data(), s.length());
		f.Close();
	}
}

bool LoadFromFile(const string& strFileName, string& s) {
	char buffer[1025]{};
	int iRead;
	CFile f(strFileName.c_str(), CFile::modeRead | CFile::typeBinary);
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
