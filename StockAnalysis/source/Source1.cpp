BOOL bNameValid;
// Create output window
CString strOutputWnd;
bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
ASSERT(bNameValid);
if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
{
	TRACE0("Failed to create Output window\n");
	return FALSE; // failed to create
}

SetDockingWindowIcons(theApp.m_bHiColorIcons);
return TRUE;
