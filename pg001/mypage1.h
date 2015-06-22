#ifndef IDD_PROPPAGE_SMALL
#include"resource.h"
#endif
#define ID_MYPAGE1

class mypage1:public CPropertyPage
{
public:
	DECLARE_DYNCREATE(mypage1)
	mypage1():CPropertyPage(IDD_PROPPAGE_SMALL2){};
	BOOL OnInitDialog();
	afx_msg void onadd();
	afx_msg void onchk(); //自动刷新单选按钮的响应函数
	LRESULT onacc(WPARAM wprarm,LPARAM lparam); //来自线程消息的响应函数
	void  mod_chk();//提供主界面调用的函数
public:
DECLARE_MESSAGE_MAP()
};

