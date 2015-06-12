#include"stdafx.h"
#include"mypage2.h"
#include"mystruct.h"

IMPLEMENT_DYNCREATE(mypage2,CPropertyPage)

BEGIN_MESSAGE_MAP(mypage2,CPropertyPage)
	//
	ON_BN_CLICKED(IDC_BUTTON1,on_run)
	ON_BN_CLICKED(IDC_BUTTON2,on_stop)
END_MESSAGE_MAP()

BOOL mypage2::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	return true;
};

afx_msg void mypage2::on_run()
{

};
afx_msg void mypage2::on_stop()
{

};

