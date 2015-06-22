#include"stdafx.h"
#include"mypage1.h"
#include"mystruct.h"
#include"myconst.h"

	
extern int keep_acc;
extern int thd_running;
extern int imm_code;
extern data_tab dtab;
extern HWND page1_wnd;
extern int gpcode[8][2];
extern UINT get_data_imm(LPVOID lpvoid);


/*PUTMSG putmsg=NULL;
GETMSG getmsg=NULL;
DELMSG delmsg=NULL;
DELDATA	deldata=NULL; */



IMPLEMENT_DYNCREATE(mypage1,CPropertyPage)

BEGIN_MESSAGE_MAP(mypage1,CPropertyPage)
	//
	ON_BN_CLICKED(IDC_BUTTON1,onadd)
	ON_MESSAGE(WM_IMM_MESSAGE,onacc)
	ON_BN_CLICKED(IDC_CHECK1,onchk)
END_MESSAGE_MAP()

BOOL mypage1::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	page1_wnd=this->GetSafeHwnd();
	return true;
};
afx_msg void mypage1::onadd()
{
	CString str;
	int i,j,k;
	GetDlgItemText(IDC_EDIT1,str);
	if(str.IsEmpty())
	{
		MessageBox("请输入待查询的股票代码");
		return;
	}
	i=atoi(str.GetBuffer(0));
	k=0;
	for(j=0;j<8;j++)
	{//保证输入的代码在有效的范围内
		if(i>=gpcode[j][0] && i<gpcode[j][1])
		{k=1;break;}
	}
	if(k==0)
	{
		MessageBox("输入的代码有误，请检查输入的正确性");
		return;
	}
	imm_code=i;
	if(thd_running == 0)
		AfxBeginThread(get_data_imm,NULL);
};
LRESULT mypage1::onacc(WPARAM wprarm,LPARAM lparam)
{
	CString str;
	str.Format("%s",dtab.gp_name);
	SetDlgItemText(IDC_EDIT2,str);		//输出名称
	str=(dtab.gp_type==0?"沪市":"深市");
	SetDlgItemText(IDC_EDIT10,str);		//输出交易所
	str.Format("%0.2f",dtab.beg);
	SetDlgItemText(IDC_EDIT11,str);		//开盘价
	str.Format("%0.2f",dtab.cur);
	SetDlgItemText(IDC_EDIT12,str);		//当前价
	str.Format("%0.2f",dtab.cur_max);
	SetDlgItemText(IDC_EDIT13,str);		//最高价
	str.Format("%0.2f",dtab.cur_min);
	SetDlgItemText(IDC_EDIT14,str);		//最低价
	str.Format("%s",dtab.total_cnt);
	SetDlgItemText(IDC_EDIT15,str);		//成交量
	str.Format("%s",dtab.total_pri);
	SetDlgItemText(IDC_EDIT16,str);		//成交价
	str=dtab.gp_date.Format("20%y-%m-%d %H:%M:%S");
	SetDlgItemText(IDC_EDIT4,str);		//时间
	return 0;
};
afx_msg void mypage1::onchk()
{
	CButton *bt=(CButton*)GetDlgItem(IDC_CHECK1);
	keep_acc=bt->GetCheck();
};
void mypage1::mod_chk()
{
	CButton *bt=(CButton*)GetDlgItem(IDC_CHECK1);
	bt->SetCheck(0);
	keep_acc=0;
};
