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
	afx_msg void onchk(); //�Զ�ˢ�µ�ѡ��ť����Ӧ����
	LRESULT onacc(WPARAM wprarm,LPARAM lparam); //�����߳���Ϣ����Ӧ����
	void  mod_chk();//�ṩ��������õĺ���
public:
DECLARE_MESSAGE_MAP()
};

