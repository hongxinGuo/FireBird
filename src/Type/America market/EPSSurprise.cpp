module;

module FireBirdLib.EPSSurprise;

CEPSSurprise::CEPSSurprise() {
	m_strSymbol = " ";
	m_lDate = local_days(std::chrono::days(0));
	m_dActual = 0.0;
	m_dEstimate = 0.0;
}
