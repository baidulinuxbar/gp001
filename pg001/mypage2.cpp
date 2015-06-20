#include"stdafx.h"
#include"mypage2.h"
#include"mystruct.h"
#include"myconst.h"


extern HWND page2_wnd;
//获取数据的线程函数
extern UINT get_data(LPVOID lpvoid);



IMPLEMENT_DYNCREATE(mypage2,CPropertyPage)

BEGIN_MESSAGE_MAP(mypage2,CPropertyPage)
	//
	ON_BN_CLICKED(IDC_BUTTON1,on_run)
	ON_BN_CLICKED(IDC_BUTTON2,on_stop)
	ON_MESSAGE(WM_TEST_MESSAGE,ontest)
END_MESSAGE_MAP()

BOOL mypage2::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	page2_wnd=this->GetSafeHwnd();
	return true;
};

afx_msg void mypage2::on_run()
{
	cnts=0;
	AfxBeginThread(get_data,NULL);
};
afx_msg void mypage2::on_stop()
{

};

LRESULT mypage2::ontest(WPARAM wparam,LPARAM lparam)
{
	CString str,s1;
	if(lparam==NULL)
		SetDlgItemText(IDC_EDIT1,"");
	else
	{
		GetDlgItemText(IDC_EDIT1,s1);
		str.Format("\r\n%s",(char*)lparam);
		s1+=str;
		SetDlgItemText(IDC_EDIT1,s1);
		str.Format("记录数：%d",++cnts);
		SetDlgItemText(IDC_STATIC1,str);
	}
	return 0;
};
