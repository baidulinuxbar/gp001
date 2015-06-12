#include"stdafx.h"
#include"mypage1.h"
#include"mystruct.h"

extern bmsg g_msg;
extern data_v g_data;


IMPLEMENT_DYNCREATE(mypage1,CPropertyPage)

BEGIN_MESSAGE_MAP(mypage1,CPropertyPage)
	//
	ON_BN_CLICKED(IDC_BUTTON1,onadd)
	ON_BN_CLICKED(IDC_BUTTON3,ondel)
END_MESSAGE_MAP()

BOOL mypage1::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CButton *bt=(CButton*)GetDlgItem(IDC_RADIO1);
	bt->SetCheck(1);
	return true;
};
afx_msg void mypage1::onadd()
{
	CString st1,str;
	memset((void*)&g_msg,0,sizeof(g_msg));
	GetDlgItemText(IDC_EDIT1,str);
	if(str.IsEmpty())
	{
		MessageBox("请输入股票代码");
		return;
	}
	g_msg.code=atoi(str.GetBuffer(0));
	GetDlgItemText(IDC_EDIT2,str);
	if(str.IsEmpty())
	{
		MessageBox("请输入股票名称");
		return;
	}
	memcpy((void*)g_msg.name,str.GetBuffer(0),str.GetLength());
	CButton *bt=(CButton*)GetDlgItem(IDC_RADIO1);
	g_msg.gtype=bt->GetCheck();
	bt=(CButton*)GetDlgItem(IDC_CHECK1);
	if(!bt->GetCheck())
		return;
	GetDlgItemText(IDC_EDIT3,str);
	if(str.IsEmpty())
	{
		MessageBox("请输入波峰值");
		return;
	}
	g_msg.gwatch=0;
	g_msg.gmax=atof(str.GetBuffer(0));
};
afx_msg  void mypage1::ondel()
{};