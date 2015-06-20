#ifndef IDD_PROPPAGE_SMALL
#include"resource.h"
#endif
#define ID_MYPAGE1

class mypage1:public CPropertyPage
{
public:
	DECLARE_DYNCREATE(mypage1)
	mypage1():CPropertyPage(IDD_PROPPAGE_SMALL){};
	BOOL OnInitDialog();
	afx_msg void onadd();
	afx_msg void ondel();
	afx_msg void onselchg();
private:
	BOOL del_result(int ncode);
public:
DECLARE_MESSAGE_MAP()
};

