#include"stdafx.h"
#include"mypage1.h"
#include"mystruct.h"

extern bmsg g_msg;
extern data_v g_data;
extern CArray<bmsg,bmsg&> gamsg;

PUTMSG putmsg=NULL;
GETMSG getmsg=NULL;
DELMSG delmsg=NULL;
CArray<bmsg,bmsg&> *amsg=NULL;


IMPLEMENT_DYNCREATE(mypage1,CPropertyPage)

BEGIN_MESSAGE_MAP(mypage1,CPropertyPage)
	//
	ON_BN_CLICKED(IDC_BUTTON1,onadd)
	ON_BN_CLICKED(IDC_BUTTON3,ondel)
	ON_LBN_DBLCLK(IDC_LIST1,onselchg)
END_MESSAGE_MAP()

BOOL mypage1::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CButton *bt=(CButton*)GetDlgItem(IDC_RADIO1);
	bt->SetCheck(1);
	CComboBox *box=(CComboBox*)GetDlgItem(IDC_COMBO1);
	box->ResetContent();
	box->AddString("3天");
	box->AddString("4天");
	box->AddString("5天");
	box->AddString("10天");
	box->AddString("15天");
	box->AddString("20天");
	box->SetCurSel(2);
	box=(CComboBox*)GetDlgItem(IDC_COMBO2);
	box->ResetContent();
	box->AddString("80%");
	box->AddString("70%");
	box->AddString("60%");
	box->AddString("50%");
	box->AddString("40%");
	box->AddString("30%");
	box->SetCurSel(2);
	int i,j;
	CString str;
	CListBox *list=(CListBox*)GetDlgItem(IDC_LIST1);
	list->ResetContent();
	HMODULE mod=LoadLibrary("gpdll.dll");
	if(mod==NULL)
	{
		MessageBox("加载动态俩接库失败！");
		PostQuitMessage(0);
	}
	getmsg=(GETMSG)::GetProcAddress(mod,"getmsg");
	if(getmsg==NULL)
	{
		::FreeLibrary(mod);
		MessageBox("取得函数地址失败!");
		PostQuitMessage(0);
	}
	if(getmsg(NULL)!=0)
	{
		::FreeLibrary(mod);
		MessageBox("取得数据错误！");
		PostQuitMessage(0);
	}
	amsg=(CArray<bmsg,bmsg&> *)::GetProcAddress(mod,"dlmsg");
	if(amsg==NULL)
	{
		::FreeLibrary(mod);
		MessageBox("取得函数地址失败");
		PostQuitMessage(0);
	}
	gamsg.RemoveAll();
	i=amsg->GetCount();
	for(j=0;j<i;j++)
	{
		memset((void*)&g_msg,0,sizeof(g_msg));
		g_msg=amsg->GetAt(j);
		str.Format("%s",g_msg.name);
		list->AddString(str);
		gamsg.Add(g_msg);
	}
	::FreeLibrary(mod);
	return true;
};
afx_msg void mypage1::onadd()
{
	CString str;
	HMODULE mod;
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
//	g_msg.name=str;
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
	GetDlgItemText(IDC_EDIT4,str);
	if(str.IsEmpty())
	{
		MessageBox("请输入波谷值");
		return;
	}
	g_msg.gstd=atof(str.GetBuffer(0));
	CComboBox *box=(CComboBox*)GetDlgItem(IDC_COMBO1);
	g_msg.gcycle=box->GetCurSel();
	box=(CComboBox*)GetDlgItem(IDC_COMBO2);
	g_msg.gdecay=box->GetCurSel();
	mod=LoadLibrary("gpdll.dll");
	if(mod==NULL)
	{
		MessageBox("加载动态连接库失败");
		return;
	}
	putmsg=(PUTMSG)::GetProcAddress(mod,"putmsg");
	if(putmsg==NULL)
	{
		::FreeLibrary(mod);
		MessageBox("取得函数地址失败");
		return;
	}
	int i=putmsg((void*)&g_msg);
	::FreeLibrary(mod);
	if(i==0)
	{
		CListBox *box=(CListBox*)GetDlgItem(IDC_LIST1);
		box->AddString(g_msg.name);
	}	
};
afx_msg  void mypage1::ondel()
{
	CListBox *list=(CListBox*)GetDlgItem(IDC_LIST1);
	int i,j;
	i=list->GetCurSel();
	list->DeleteString(i);

};

afx_msg void mypage1::onselchg()
{
	int i;
	CString str;
	CListBox *list=(CListBox*)GetDlgItem(IDC_LIST1);
	i=list->GetCurSel();
	memset((void*)&g_msg,0,sizeof(g_msg));
	g_msg=gamsg.GetAt(i);
	str.Format("%d",g_msg.code);
	SetDlgItemText(IDC_EDIT1,str);
	str.Format("%s",g_msg.name);
	SetDlgItemText(IDC_EDIT2,str);
	CButton *bt=(CButton*)GetDlgItem(IDC_RADIO1);
	if(g_msg.gtype==0)
	{
		bt->SetCheck(1);
		bt=(CButton*)GetDlgItem(IDC_RADIO2);
		bt->SetCheck(0);
	}
	else
	{
		bt->SetCheck(0);
		bt=(CButton*)GetDlgItem(IDC_RADIO2);
		bt->SetCheck(1);
	}
	bt=(CButton*)GetDlgItem(IDC_CHECK1);
	if(g_msg.gwatch==0)//三重顶
		bt->SetCheck(0);
	else
		bt->SetCheck(1);
	str.Format("%0.2f",g_msg.gmax);
	SetDlgItemText(IDC_EDIT3,str);
	str.Format("%0.2f",g_msg.gstd);
	SetDlgItemText(IDC_EDIT4,str);
	CComboBox *box=(CComboBox*)GetDlgItem(IDC_COMBO1);
	box->SetCurSel(g_msg.gcycle);
	box=(CComboBox*)GetDlgItem(IDC_COMBO2);
	box->SetCurSel(g_msg.gdecay);
};