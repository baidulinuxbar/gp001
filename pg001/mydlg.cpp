#include"stdafx.h"
#include"mydlg.h"
#include"mystruct.h"
#include"myconst.h"

#pragma comment(lib,"ws2_32.lib")

#define NOTIFYICON_OWN_ID     0X33
#define WM_NOTIFY_BACK        WM_USER+0X101
#define IDC_QUIT              40050
#define IDC_ZHANWANG          40053
#define IDC_SHOWMAIN          40054
#define IDC_SHUOMING		  40055

extern code_tab ctab;	
CArray<code_tab,code_tab&> *amsg=NULL;

BEGIN_MESSAGE_MAP(mydlg,CDialog)
	//
	ON_NOTIFY(TCN_SELCHANGE,IDC_TAB1,onsel)
	ON_MESSAGE(WM_NOTIFY_BACK,onnotify)
	ON_COMMAND(IDC_QUIT,onquit)
//	ON_COMMAND(IDC_ZHANWANG,onshow)
	ON_COMMAND(IDC_SHOWMAIN,onshow)
	ON_COMMAND(IDC_SHUOMING,onparam)
END_MESSAGE_MAP()

BOOL mydlg::OnInitDialog()
{
	WSADATA wd;
	int i;
	CDialog::OnInitDialog();
	if(read_dt()!=0)
		PostQuitMessage(0);
	SetIcon(AfxGetApp()->LoadIconA(IDI_ICON1),TRUE);
	SetIcon(AfxGetApp()->LoadIconA(IDI_ICON1),FALSE);
	i=WSAStartup(MAKEWORD(2,2),&wd);
	if(i!=0)
	{
		MessageBox("初始化套接字失败");
		PostQuitMessage(0);
	}
	if(LOBYTE(wd.wVersion)!=2 || HIBYTE(wd.wVersion)!=2)
	{
		MessageBox("套接字版本不一致");
		PostQuitMessage(0);
	}
	ni.cbSize=sizeof(NOTIFYICONDATA);
	ni.hIcon=AfxGetApp()->LoadIcon(IDI_ICON1);
	ni.hWnd=this->GetSafeHwnd();
	char ch[1024];
	int len=1024;
	memset(ch,0,sizeof(ch));
	_snprintf_s(ch,len,1024,"股票分析V0.1\nAuthor:tybitsfox\nEmail:tybitsfox@163.com");
	len=strlen(ch);
    memcpy(ni.szTip,ch,len+2);
	ni.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE;
	ni.uID=NOTIFYICON_OWN_ID;
	ni.uCallbackMessage=WM_NOTIFY_BACK;
//	ni.dwState=NIS_HIDDEN;
	Shell_NotifyIcon(NIM_ADD,&ni);

	CTabCtrl *tab=(CTabCtrl*)GetDlgItem(IDC_TAB1);
	tab->InsertItem(0,"即时查询");
	tab->InsertItem(1,"数据抓取");
	tab->InsertItem(2,"历史查询");
	CRect rect;
	tab->GetClientRect(&rect);
	rect.top+=30;rect.bottom-=2;
	rect.left+=4;rect.right-=4;
	mp1.Create(IDD_PROPPAGE_SMALL2,tab);
	mp1.MoveWindow(&rect);
	mp1.ShowWindow(TRUE);
	mp1.EnableWindow();
	mp2.Create(IDD_PROPPAGE_SMALL1,tab);
	mp2.MoveWindow(&rect);
	mp2.ShowWindow(FALSE);
	mp2.EnableWindow(0);
	mp3.Create(IDD_PROPPAGE_SMALL,tab);
	mp3.MoveWindow(&rect);
	mp3.ShowWindow(FALSE);
	mp3.EnableWindow(0);
	tab->SetCurSel(0);
	return true;
};
afx_msg void mydlg::OnOK()
{
	if(IsWindowVisible())
		ShowWindow(SW_HIDE);
//	CDialog::OnOK();
};
afx_msg void mydlg::OnCancel()
{
	if(IsWindowVisible())
		ShowWindow(SW_HIDE);
//	CDialog::OnCancel();
};
void mydlg::onsel(NMHDR *pnmhdr,LRESULT *presult)
{
	CTabCtrl *tab=(CTabCtrl *)GetDlgItem(IDC_TAB1);
	int i;
	i=tab->GetCurSel();
	switch(i)
	{
	case 0:
		mp3.ShowWindow(FALSE);
		mp3.EnableWindow(0);
		mp2.ShowWindow(FALSE);
		mp2.EnableWindow(0);
		mp1.ShowWindow(TRUE);
		mp1.EnableWindow(1);
		break;
	case 1:
		mp1.mod_chk();
		mp3.ShowWindow(FALSE);
		mp3.EnableWindow(0);
		mp1.ShowWindow(FALSE);
		mp1.EnableWindow(0);
		mp2.ShowWindow(TRUE);
		mp2.EnableWindow(1);
		break;
	case 2:
		mp1.mod_chk();
		mp1.ShowWindow(FALSE);
		mp1.EnableWindow(0);
		mp2.ShowWindow(FALSE);
		mp2.EnableWindow(0);
		mp3.ShowWindow(TRUE);
		mp3.EnableWindow(1);
		break;
	}

	presult=0;
};
int mydlg::read_dt()
{
	int i;
	HMODULE mod=NULL;
	GETMSG  get_cd=NULL;
	mod=::LoadLibraryA("gpdll.dll");
	if(mod==NULL)
	{
		MessageBox("初始化失败：加载动态连接库失败！");
		return 1;
	}
	get_cd=(GETMSG)::GetProcAddress(mod,"get_ct");
	if(get_cd==NULL)
	{
		::FreeLibrary(mod);
		MessageBox("初始化失败：取得函数地址失败！");
		return 1;
	}
	amsg=(CArray<code_tab,code_tab&> *)::GetProcAddress(mod,"dlct");
	if(amsg==NULL)
	{
		::FreeLibrary(mod);
		MessageBox("初始化失败：取得函数地址失败11！");
		return 1;
	}
	i=get_cd(NULL);
	if(i!=0)
	{
		::FreeLibrary(mod);
		return 1;
	}
	i=amsg->GetCount();
	if(i!=1)
	{
		::FreeLibrary(mod);
		return 1;
	}
	memset((void*)&ctab,0,sizeof(ctab));
	ctab=amsg->GetAt(0);
	::FreeLibrary(mod);
	return 0;
};

LRESULT mydlg::onnotify(WPARAM wparam,LPARAM lparam)
{
	if(wparam==NOTIFYICON_OWN_ID && lparam==WM_LBUTTONDBLCLK)
	{
		if(!IsWindowVisible())
			ShowWindow(SW_SHOW);
		return 0;
	}
	if(wparam==NOTIFYICON_OWN_ID && lparam==WM_RBUTTONUP)
	{
		CMenu menu;//,submenu;
		menu.CreatePopupMenu();
        menu.AppendMenu(MF_STRING,IDC_SHUOMING,"参数设置");
		menu.AppendMenu(MF_STRING,IDC_SHOWMAIN,"恢复视窗");
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING,IDC_QUIT,"退    出");
		CPoint pos;
		GetCursorPos(&pos);
		TrackPopupMenu(menu.Detach(),TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,pos.x,pos.y,0,this->GetSafeHwnd(),CRect(0,0,0,0));
	}
	return 0;
};

void mydlg::onquit()
{
	Shell_NotifyIcon(NIM_DELETE,&ni);
	WSACleanup();
	PostQuitMessage(0);
};

void mydlg::onshow()
{
	if(!IsWindowVisible())
		ShowWindow(SW_SHOW);
};
void mydlg::onparam()
{//在这里调用参数设置对话框
	MessageBox("this is a  param  dialog");
};
