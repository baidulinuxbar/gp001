#ifndef IDD_DIALOG1
#include"resource.h"
#endif
#ifndef ID_MYPAGE1
#include"mypage1.h"
#endif
#ifndef ID_MYPAGE2
#include"mypage2.h"
#endif

class mydlg:public CDialog
{
public:
	mypage1 mp1;
	mypage2 mp2;
	mydlg(CWnd *pwnd=NULL):CDialog(IDD_DIALOG1,pwnd){};
	BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	void onsel(NMHDR *pnmhdr,LRESULT *presult);//Tab控件的点击响应函数
DECLARE_MESSAGE_MAP()
};