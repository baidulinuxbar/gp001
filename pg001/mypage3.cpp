#include"stdafx.h"
#include"mypage3.h"
#include"myconst.h"
#include"mystruct.h"


IMPLEMENT_DYNCREATE(mypage3,CPropertyPage)

BEGIN_MESSAGE_MAP(mypage3,CPropertyPage)
	//
	ON_BN_CLICKED(IDC_BUTTON1,onsrh)
END_MESSAGE_MAP()

BOOL mypage3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	return true;
};
afx_msg void mypage3::onsrh()
{

};

