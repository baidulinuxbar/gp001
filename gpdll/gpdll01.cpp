#include"gpdll01.h"

//�����ģ���ڲ�ʹ�õĺ������������ݿ����ӹؼ���
BOOL crt_conn(CString &str)
{
	char ch[1024];
	CString st,s1,s;
	int i;
	DWORD len=1024;
	memset((void*)&ch,0,sizeof(ch));
	::GetModuleFileName(NULL,ch,len);
	st.Format("%s",ch);
	i=st.ReverseFind('\\');
	s1=st.Left(i+1);
	str="ODBC;Driver={Microsoft Access Driver (*.mdb, *.accdb)};DSN='''';Dbq=";
	str+=s1;str+=dbfile;
	st.Format("; pwd=tiany6806");
//	st.Format(";SystemDB=%s;UID=guoli; pwd=saike1234",s1);
	str+=st;
	return true;
};

//����������ݿ�洢�µĻ�����Ϣ�ĺ���
_declspec(dllexport) int WINAPI putmsg(void *res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	whichtab=0;
	bmsg  bt;
	if(res==NULL)
		return 1;	//error
	memset((void*)&bt,0,sizeof(bt));
	memcpy((void*)&bt,res,sizeof(bt));
	CString str;
	crt_conn(str);
	CDatabase db;
	if(db.IsOpen())
		db.Close();
	try
	{db.Open(NULL,false,false,str);}
	catch(CDBException *ep)
	{
		MessageBox(NULL,ep->m_strError,"from dll",0);
		return 1;
	}
	myrecord mrd(&db);
	if(mrd.IsOpen())
		mrd.Close();
	try
	{mrd.Open(AFX_DB_USE_DEFAULT_TYPE,"SELECT * FROM base_msg");}
	catch(CDBException *ep)
	{
		db.Close();
		MessageBox(NULL,ep->m_strError,"from dll",0);
		return 1;
	}
	mrd.AddNew();
//	memset((void*)&mrd.bsd,0,sizeof(mrd.bsd));
//	memcpy((void*)&mrd.bsd,bt,sizeof(mrd.bsd));
/*2015-6-15 ��ʹ�������ṹ��������������ֶθ�ֵ�ķ������Ա����Զ������ֶε���Ϊ�ı䡣
���ﻹ��һ��΢���Ͽɵ�bug,�����ڵ���ģʽʱ���ͻ����һ��CRecordset�ĳ�Ա��ַ�ı��ɵ�ƶ��ԣ�
�ö��Ժ�����dbrfx.cpp�ļ��У�line560����ͨ���鿴dbrfx.cpp�ļ����֣������������#ifdef _DEBUG���ж���
ִ�еģ�������release�汾�²����д��󣬵����ڵ�ʽģʽ�»���ֶ��Դ���
*/
	mrd.bsd.code=bt.code;
	mrd.bsd.gmax=bt.gmax;
	mrd.bsd.gstd=bt.gstd;
	mrd.bsd.gtimes=bt.gtimes;
	mrd.bsd.gtype=bt.gtype;
	mrd.bsd.gwatch=bt.gwatch;
	memcpy((void *)&(mrd.bsd.name),(void*)&(bt.name),60);
	mrd.bsd.gcycle=bt.gcycle;
	mrd.bsd.gdecay=bt.gdecay;
	mrd.Update();
	mrd.Close();
	db.Close();

	return 0;
};

//����Ĵ����ݿ�����Ļ�����Ϣ�ĺ���
_declspec(dllexport) int WINAPI getmsg(void *res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	if(res==NULL)
//		return 1;
	whichtab=0;
	dlmsg.RemoveAll();
	CString str,s1;
	crt_conn(str);
	CDatabase db;
	if(db.IsOpen())
		db.Close();
	try
	{db.Open(NULL,false,false,str);}
	catch(CDBException *ep)
	{
		MessageBox(NULL,ep->m_strError,"from dll",0);
		return 1;
	}
	s1.Format("SELECT * FROM base_msg");
	myrecord mrd(&db);
	if(mrd.IsOpen())
		mrd.Close();
	try
	{mrd.Open(AFX_DB_USE_DEFAULT_TYPE,s1);}
	catch(CDBException *ep)
	{
		db.Close();
		MessageBox(NULL,ep->m_strError,"from dll-001",0);
		return 1;
	}
	if(mrd.IsEOF())
	{
		mrd.Close();
		db.Close();
		return 0;
	}
	mrd.MoveFirst();
	while(!mrd.IsEOF())
	{
		dlmsg.Add(mrd.bsd);
		mrd.MoveNext();
	}
	mrd.Close();
	db.Close();
	return 0;
};

//����������ݿ�洢���ݵĺ���
_declspec(dllexport) int WINAPI putdata(void *res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(res==NULL)
		return 1;
	whichtab=1;
	CString str;
	crt_conn(str);
	CDatabase db;
	if(db.IsOpen())
		db.Close();
	try
	{db.Open(NULL,false,false,str);}
	catch(CDBException *ep)
	{
		MessageBox(NULL,ep->m_strError,"from dll-002",0);
		return 1;
	}
	myrecord mrd(&db);
	if(mrd.IsOpen())
		mrd.Close();
	try
	{mrd.Open(AFX_DB_USE_DEFAULT_TYPE,"SELECT * FROM data_msg");}
	catch(CDBException *ep)
	{
		db.Close();
		MessageBox(NULL,ep->m_strError,"from dll-003",0);
		return 1;
	}
	mrd.AddNew();
	memset((void*)&mrd.dv,0,sizeof(mrd.dv));
	memcpy((void*)&mrd.dv,res,sizeof(mrd.dv));
	mrd.Update();
	mrd.Close();
	db.Close();
	return 0;
};

//����Ĵ����ݿ��ȡ���ݵĺ���
_declspec(dllexport) int WINAPI getdata(void *res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(res==NULL)
		return 1;
	whichtab=1;
	dldata.RemoveAll();
	CString str,s1;
	crt_conn(str);
	CDatabase db;
	if(db.IsOpen())
		db.Close();
	try
	{db.Open(NULL,false,false,str);}
	catch(CDBException *ep)
	{
		MessageBox(NULL,ep->m_strError,"from dll-004",0);
		return 1;
	}
	s1.Format("%s",(char*)res);
	myrecord mrd(&db);
	if(mrd.IsOpen())
		mrd.Close();
	try
	{mrd.Open(AFX_DB_USE_DEFAULT_TYPE,s1);}
	catch(CDBException *ep)
	{
		db.Close();
		MessageBox(NULL,ep->m_strError,"from dll-005",0);
		return 1;
	}
	if(mrd.IsEOF())
	{
		mrd.Close();
		db.Close();
		return 0;
	}
	mrd.MoveFirst();
	while(!mrd.IsEOF())
	{
		dldata.Add(mrd.dv);
		mrd.MoveNext();
	}
	mrd.Close();
	db.Close();
	return 0;
};

_declspec(dllexport) int WINAPI delmsg(void *res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(res==NULL)
		return 1;
	whichtab=0;
	CString str,conn;
	crt_conn(conn);
	CDatabase db;
	if(db.IsOpen())
		db.Close();
	try
	{db.Open(NULL,false,false,conn);}
	catch(CDBException *ep)
	{
		MessageBox(NULL,ep->m_strError,"from dll-005",0);
		return 1;
	}
	str.Format("%s",(char *)res);
	myrecord mrd(&db);
	if(mrd.IsOpen())
		mrd.Close();
	try
	{mrd.Open(AFX_DB_USE_DEFAULT_TYPE,str);}
	catch(CDBException *ep)
	{
		MessageBox(NULL,ep->m_strError,"from dll-006",0);
		db.Close();
		return 1;
	}
	if(mrd.IsEOF() || !mrd.CanUpdate() || !mrd.CanTransact())
	{
		MessageBox(NULL,"delete result error!","from dll-007",0);
		mrd.Close();
		db.Close();
		return 1;
	}
	mrd.Delete();
	db.CommitTrans();
	mrd.Close();
	db.Close();
	return 0;
};

//�����ɾ�����ݵĺ���
_declspec(dllexport) int WINAPI deldata(void *res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(res==NULL)
		return 1;
	whichtab=1;
	CString str,conn;
	crt_conn(conn);
	CDatabase db;
	if(db.IsOpen())
		db.Close();
	try
	{db.Open(NULL,false,false,conn);}
	catch(CDBException *ep)
	{
		MessageBox(NULL,ep->m_strError,"from dll-008",0);
		return 1;
	}
	str.Format("%s",(char*)res);
	myrecord mrd(&db);
	if(mrd.IsOpen())
		mrd.Close();
	try
	{mrd.Open(AFX_DB_USE_DEFAULT_TYPE,str);}
	catch(CDBException *ep)
	{
		db.Close();
		MessageBox(NULL,ep->m_strError,"from dll-010",0);
		return 1;
	}
	if(mrd.IsEOF() || !mrd.CanUpdate() || !mrd.CanTransact())
	{
		MessageBox(NULL,"delete result error!","from dll-009",0);
		mrd.Close();
		db.Close();
		return 1;
	}
/*	mrd.MoveFirst();
	while(!mrd.IsEOF())
	{
		mrd.Delete();
		mrd.MoveNext();
	}
*/
	mrd.Delete();
	db.CommitTrans();
	mrd.Close();
	db.Close();
	return 0;
};

//����ı��3 code_tab�Ĵ洢�������ú��������ڼ�¼����������
/*2015-6-21�޸ģ��ú������������������Դ���Ĳ������Ƕ��ж���һ���ṹָ��*/
_declspec(dllexport) int WINAPI put_ct(void *res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(res==NULL)
		return 1;
	whichtab=2;
	CString str;
	crt_conn(str);
	CDatabase db;
	if(db.IsOpen())
		db.Close();
	try
	{db.Open(NULL,false,false,str);}
	catch(CDBException *ep)
	{
		MessageBox(NULL,ep->m_strError,"from dll-1001",0);
		return 1;
	}
	myrecord mrd(&db);
	if(mrd.IsOpen())
		mrd.Close();
	try
	{mrd.Open(AFX_DB_USE_DEFAULT_TYPE,"SELECT * FROM code_tab");}
	catch(CDBException *ep)
	{
		db.Close();
		MessageBox(NULL,ep->m_strError,"from dll-1002",0);
		return 1;
	}
/*	mrd.AddNew();
	memset((void*)&(mrd.ct),0,sizeof(mrd.ct));
	memcpy((void*)&(mrd.ct),res,sizeof(mrd.ct)); */
	mrd.MoveFirst();
	mrd.Edit();
	memcpy((void*)&(mrd.ct),res,sizeof(mrd.ct));
	mrd.Update();
	mrd.Close();
	db.Close();
	return 0;
};
//����ı��4 data_tab�Ĵ洢�������ú���Ҳ���ڼ�¼����������
_declspec(dllexport) int WINAPI put_dt(void *res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(res==NULL)
		return 1;
	whichtab=3;
	CArray<data_tab,data_tab&> *ca;
	ca=(CArray<data_tab,data_tab&> *)res;
	CString str;
	int i,j;
	crt_conn(str);
	CDatabase db;
	if(db.IsOpen())
		db.Close();
	try
	{db.Open(NULL,false,false,str);}
	catch(CDBException *ep)
	{
		MessageBox(NULL,ep->m_strError,"from dll-1003",0);
		return 1;
	}
	myrecord mrd(&db);
	if(mrd.IsOpen())
		mrd.Close();
	try
	{mrd.Open(AFX_DB_USE_DEFAULT_TYPE,"SELECT * FROM data_tab",0);}
	catch(CDBException *ep)
	{
		db.Close();
		MessageBox(NULL,ep->m_strError,"from dll-1004",0);
		return 1;
	}
	i=ca->GetCount();
	for(j=0;j<i;j++)
	{
		mrd.AddNew();
		memset((void*)&(mrd.dt),0,sizeof(mrd.dt));
		mrd.dt=ca->GetAt(j);
		mrd.Update();
	}
	mrd.Close();
	db.Close();
	return 0;
};
//����ı��3�Ķ�ȡ����
/*2015-6-21�޸ģ�ԭ���ú������贫����������贫��һ��__time64_t�Ĳ���,�ò�������ȷ��
��ѯ�������Ƿ���ڼ�¼���ú������ܷ���0��¼����ʾ������û�л�ȡ��¼��
2015-6-22�޸ģ�����ֻ��Ψһһ����¼���漰�Ĳ���ֻ�У���ȡ�͸���
*/
_declspec(dllexport) int WINAPI get_ct(void *res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	whichtab=2;
	dlct.RemoveAll();
	CString str;
	crt_conn(str);
	CDatabase db;
	if(db.IsOpen())
		db.Close();
	try
	{db.Open(NULL,false,false,str);}
	catch(CDBException *ep)
	{
		MessageBox(NULL,ep->m_strError,"from dll-1005",0);
		return 1;
	}
	str.Format("SELECT * FROM code_tab");
	myrecord mrd(&db);
	if(mrd.IsOpen())
		mrd.Close();
	try
	{mrd.Open(AFX_DB_USE_DEFAULT_TYPE,str,0);}
	catch(CDBException *ep)
	{
		db.Close();
		MessageBox(NULL,ep->m_strError,"from dll-1006",0);
		return 1;
	}
	if(mrd.IsEOF())
	{
		mrd.Close();
		db.Close();
		return 0;
	}
	mrd.MoveFirst();
	while(!mrd.IsEOF())
	{
		dlct.Add(mrd.ct);
		mrd.MoveNext();
	}
	mrd.Close();
	db.Close();
	return 0;
};
//����ı��4�Ķ�ȡ����
/*2015-6-21�޸ģ��޸�Ϊ����ѯ����ȡ�ü�¼��������ѯ���ֱ���ں����⹹�죬��ֱ��ͨ����������
�ڱ�����ֱ��ʹ�á�*/
_declspec(dllexport) int WINAPI get_dt(void *res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(res==NULL)
		return 1;
	whichtab=3;
	dldt.RemoveAll();
	CString str;
	crt_conn(str);
	CDatabase db;
	if(db.IsOpen())
		db.Close();
	try
	{db.Open(NULL,false,false,str);}
	catch(CDBException *ep)
	{
		MessageBox(NULL,ep->m_strError,"from dll-1007",0);
		return 1;
	}
	str.Format("%s",(char*)res,strlen((char*)res));
	myrecord mrd(&db);
	if(mrd.IsOpen())
		mrd.Close();
	try
	{mrd.Open(AFX_DB_USE_DEFAULT_TYPE,str,0);}
	catch(CDBException *ep)
	{
		db.Close();
		MessageBox(NULL,ep->m_strError,"from dll-1008",0);
		return 1;
	}
	if(mrd.IsEOF())
	{
		mrd.Close();
		db.Close();
		return 0;
	}
	mrd.MoveFirst();
	while(!mrd.IsEOF())
	{
		dldt.Add(mrd.dt);
		mrd.MoveNext();
	}
	mrd.Close();
	db.Close();
	return 0;
};






































