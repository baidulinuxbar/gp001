#include"stdafx.h"
#include"mydlg.h"

BEGIN_MESSAGE_MAP(mydlg,CDialog)
	//
	ON_NOTIFY(TCN_SELCHANGE,IDC_TAB1,onsel)
END_MESSAGE_MAP()

BOOL mydlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CTabCtrl *tab=(CTabCtrl*)GetDlgItem(IDC_TAB1);
	tab->InsertItem(0,"参数设置");
	tab->InsertItem(1,"数据抓取");
	CRect rect;
	tab->GetClientRect(&rect);
	rect.top+=30;rect.bottom-=2;
	rect.left+=4;rect.right-=4;
	mp1.Create(IDD_PROPPAGE_SMALL,tab);
	mp1.MoveWindow(&rect);
	mp1.ShowWindow(TRUE);
	mp1.EnableWindow();
	mp2.Create(IDD_PROPPAGE_SMALL1,tab);
	mp2.MoveWindow(&rect);
	mp2.ShowWindow(FALSE);
	mp2.EnableWindow(0);
	tab->SetCurSel(0);
	return true;
};
afx_msg void mydlg::OnOK()
{
	CDialog::OnOK();
};
afx_msg void mydlg::OnCancel()
{
	CDialog::OnCancel();
};
void mydlg::onsel(NMHDR *pnmhdr,LRESULT *presult)
{
	CTabCtrl *tab=(CTabCtrl *)GetDlgItem(IDC_TAB1);
	int i,j,k;
	i=tab->GetCurSel();
	switch(i)
	{
	case 0:
		mp2.ShowWindow(FALSE);
		mp2.EnableWindow(0);
		mp1.ShowWindow(TRUE);
		mp1.EnableWindow(1);
		break;
	case 1:
		mp1.ShowWindow(FALSE);
		mp1.EnableWindow(0);
		mp2.ShowWindow(TRUE);
		mp2.EnableWindow(1);
		break;
	}

	presult=0;
};