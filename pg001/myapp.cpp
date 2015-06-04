#include"stdafx.h"
#include"resource.h"
#include"mydlg.h"
#include"myapp.h"

BOOL myapp::InitInstance()
{
	mydlg mdlg;
	m_pMainWnd=&mdlg;
	mdlg.DoModal();
	return false;
};