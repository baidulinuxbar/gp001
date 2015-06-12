#include"gpdll01.h"

//定义的模块内部使用的函数，建立数据库连接关键字
BOOL crt_conn(CString &str)
{
	char ch[1024];
	CString st,s1,s;
	int i,j,k;
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

//定义的向数据库存储新的基本信息的函数
_declspec(dllexport) int WINAPI putmsg(void *res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	whichtab=0;
	bmsg *bt=(bmsg*)res;
	if(bt==NULL)
		return 1;	//error
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
	memset((void*)&mrd.bsd,0,sizeof(mrd.bsd));
	memcpy((void*)&mrd.bsd,(void*)bt,sizeof(mrd.bsd));
	mrd.Update();
	mrd.Close();
	db.Close();
	return 0;
};

//定义的从数据库读出的基本信息的函数
_declspec(dllexport) int WINAPI getmsg(void *res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(res==NULL)
		return 1;
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
	s1.Format("%s",(char*)res);
	myrecord mrd(&db);
	if(mrd.IsOpen())
		mrd.Close();
	try
	{mrd.Open(AFX_DB_USE_DEFAULT_TYPE,s1);}
	catch(CDBException *ep)
	{
		db.Close();
		MessageBox(NULL,ep->m_strError,"from dll-001",0);
		return;
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

//定义的向数据库存储数据的函数
_declspec(dllexport) int WINAPI putdata(void *res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(res==NULL)
		return 1;
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

//定义的从数据库读取数据的函数
_declspec(dllexport) int WINAPI getdata(void *res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(res==NULL)
		return 1;
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

	}
};



























