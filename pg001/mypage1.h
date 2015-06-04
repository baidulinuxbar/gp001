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
DECLARE_MESSAGE_MAP()
};

