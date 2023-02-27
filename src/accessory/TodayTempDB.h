#pragma once

class CTodayTempDB {
public:
	CTodayTempDB();
	~CTodayTempDB();

	bool DeleteCurrentContent();

private:
	void OpenDatabase();

protected:
	CDatabase databaseTemp;
};
