#include"pch.h"
module;
#include"SetSICIndustry.h"
export module FireBird.Type.SICIndustry;

export {
	class CSICIndustry final {
	public:
		CSICIndustry();
		~CSICIndustry() = default;

		void Append(CSetSICIndustry& setSICIndustry) const;
		void Load(const CSetSICIndustry& setSICIndustry);

		void SetUpdated(const bool fFlag) noexcept { m_fUpdated = fFlag; }
		bool IsUpdated() const noexcept { return m_fUpdated; }

	public:
		long m_lCode;
		CString m_strIndustry;
		CString m_strSector;

		bool m_fUpdated;
	};
	using CSICIndustryPtr = shared_ptr<CSICIndustry>;
}
