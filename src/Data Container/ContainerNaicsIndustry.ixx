#include"pch.h"
module;
#include"NaicsIndustry.h"
export module FireBird.Container.NaicsIndustry;

export {
	class CContainerNaicsIndustry final {
	public:
		CContainerNaicsIndustry();
		CContainerNaicsIndustry(const CContainerNaicsIndustry& other) = delete;
		CContainerNaicsIndustry(CContainerNaicsIndustry&& other) noexcept = delete;
		CContainerNaicsIndustry& operator=(const CContainerNaicsIndustry& other) = delete;
		CContainerNaicsIndustry& operator=(CContainerNaicsIndustry&& other) noexcept = delete;
		~CContainerNaicsIndustry() = default;
		void Reset();

		size_t GetTotalNaicsIndustry() const noexcept { return m_vNaicsIndustry.size(); }
		auto GetLastTotalNaicsIndustry() const noexcept { return m_lLastTotalNaicsIndustry; }

		bool IsNaicsIndustry(const CString& strNaicsIndustry) const { return !m_mapNaicsIndustry.contains(strNaicsIndustry); }

		bool IsNaicsIndustry(const CNaicsIndustryPtr& pNaicsIndustry) const { return IsNaicsIndustry(pNaicsIndustry->m_strNaics); }
		void Add(CNaicsIndustryPtr pNaicsIndustry);
		bool Delete(CNaicsIndustryPtr pNaicsIndustry);

		bool UpdateDB();
		bool LoadDB();

	protected:
		vector<CNaicsIndustryPtr> m_vNaicsIndustry;
		map<CString, size_t> m_mapNaicsIndustry;
		size_t m_lLastTotalNaicsIndustry;
	};
}
