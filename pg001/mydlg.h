#ifndef IDD_DIALOG1
#include"resource.h"
#endif
#ifndef ID_MYPAGE1
#include"mypage1.h"
#endif
#ifndef ID_MYPAGE2
#include"mypage2.h"
#endif
#ifndef ID_MYPAGE3
#include"mypage3.h"
#endif


class mydlg:public CDialog
{
public:
	mypage1 mp1;
	mypage2 mp2;
	mypage3 mp3;
	NOTIFYICONDATA ni;
	mydlg(CWnd *pwnd=NULL):CDialog(IDD_DIALOG1,pwnd){};
	BOOL OnInitDialog();
private:
	BOOL read_dt();
	afx_msg void onquit();
	afx_msg void onshow();
	afx_msg void onparam();
public:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	void onsel(NMHDR *pnmhdr,LRESULT *presult);//Tab控件的点击响应函数
	LRESULT onnotify(WPARAM wparam, LPARAM lparam);
DECLARE_MESSAGE_MAP()
};
