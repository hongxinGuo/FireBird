#pragma once

#include"FinnhubForexSymbol.h"
#include"FinnhubCryptoSymbol.h"

using namespace std;
#include<vector>
#include<map>

template <typename T> class CTemplateDataStake {
public:
	typedef typename std::vector<T>::size_type size_type;
	CTemplateDataStake() { Reset(); }
	~CTemplateDataStake() {}
	void Reset(void) { m_vStake.resize(0); m_mapStake.clear(); m_lLastTotalStake = 0; }

	size_type GetTotalStake(void) const { return m_vStake.size(); }
	bool IsStake(CString strSymbol) { if (m_mapStake.find(strSymbol) == m_mapStake.end()) return false; else return true; }
	T GetStake(long lIndex) { return m_vStake.at(lIndex); }
	T GetStake(CString strTicker) { return m_vStake.at(m_mapStake.at(strTicker)); }
	void Add(T pStake, CString strSymbol) {
		if (!IsStake(strSymbol)) {
			m_mapStake[strSymbol] = m_vStake.size();
			m_vStake.push_back(T);
		}
	}
	bool Delete(T pStake, CString strSymbol) {
		if (IsStake(strSymbol)) {
			m_vStake.erase(m_vStake.begin() + m_mapStake.at(strSymbol));
			m_mapStake.erase(strSymbol);
		}
	}

	//bool UpdateDB(void);
	//bool LoadDB(void);

protected:
	vector<T> m_vStake;
	map<CString, long> m_mapStake;
	long m_lLastTotalStake;
};
