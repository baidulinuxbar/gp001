#ifndef IDD_PROPPAGE_SMALL1
#include"resource.h"
#endif

#define ID_MYPAGE2

class mypage2:public CPropertyPage
{
public:
	DECLARE_DYNCREATE(mypage2)
	mypage2():CPropertyPage(IDD_PROPPAGE_SMALL1){}
	BOOL OnInitDialog();
	afx_msg void on_run();
	afx_msg void on_stop();
DECLARE_MESSAGE_MAP()
};