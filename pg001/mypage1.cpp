#include"stdafx.h"
#include"mypage1.h"
#include"mystruct.h"

extern bmsg g_msg;
extern data_v g_data;
extern CArray<bmsg,bmsg&> gamsg;

PUTMSG putmsg=NULL;
GETMSG getmsg=NULL;
DELMSG delmsg=NULL;
DELDATA	deldata=NULL;
CArray<bmsg,bmsg&> *amsg=NULL;


IMPLEMENT_DYNCREATE(mypage1,CPropertyPage)

BEGIN_MESSAGE_MAP(mypage1,CPropertyPage)
	//
	ON_BN_CLICKED(IDC_BUTTON1,onadd)
//	ON_BN_CLICKED(IDC_BUTTON3,ondel)
//	ON_LBN_DBLCLK(IDC_LIST1,onselchg)
END_MESSAGE_MAP()

BOOL mypage1::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	return true;
};
afx_msg void mypage1::onadd()
{
};
/*
afx_msg  void mypage1::ondel()
{
};

afx_msg void mypage1::onselchg()
{
};

BOOL mypage1::del_result(int ncode)
{
	return true;
};
*/