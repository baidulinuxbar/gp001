#ifndef IDD_DIALOG1
#include"resource.h"
#endif
#define ID_MYPAGE3

class mypage3:public CPropertyPage
{
public:
	DECLARE_DYNCREATE(mypage3)
	mypage3():CPropertyPage(IDD_PROPPAGE_SMALL){};
	BOOL OnInitDialog();
	afx_msg void onsrh(); //查询按钮的响应函数


public:
DECLARE_MESSAGE_MAP()
};