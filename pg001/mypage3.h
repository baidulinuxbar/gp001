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
	afx_msg void onsrh(); //��ѯ��ť����Ӧ����


public:
DECLARE_MESSAGE_MAP()
};