#include"stdafx.h"
#include"resource.h"
#include"mydlg.h"
#include"myapp.h"
#include"mystruct.h"

bmsg g_msg;
data_v g_data;
CArray<bmsg,bmsg&> gamsg;


BOOL myapp::InitInstance()
{
	mydlg mdlg;
	m_pMainWnd=&mdlg;
	mdlg.DoModal();
	return false;
};