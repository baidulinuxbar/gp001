#include"gpdll01.h"

//定义的模块内部使用的函数，建立数据库连接关键字
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

//定义的向数据库存储新的基本信息的函数
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
/*2015-6-15 不使用整个结构拷贝，而是逐个字段赋值的方法可以避免自动增量字段的人为改变。
这里还有一个微软认可的bug,就是在调试模式时，就会出现一个CRecordset的成员地址改变的傻逼断言，
该断言好像在dbrfx.cpp文件中（line560），通过查看dbrfx.cpp文件发现，这个断言是在#ifdef _DEBUG的判断下
执行的，所以在release版本下不会有错误，但是在调式模式下会出现断言错误。
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

//定义的从数据库读出的基本信息的函数
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

//定义的向数据库存储数据的函数
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

//定义的从数据库读取数据的函数
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

//定义的删除数据的函数
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
	mrd.Delete();
	db.CommitTrans();
	mrd.Close();
	db.Close();
	return 0;
};
























