#include"stdafx.h"
#include"resource.h"
#include"mydlg.h"
#include"myapp.h"
#include"mystruct.h"
#include"myconst.h"
#include"zconf.h"
#include"zlib.h"

//#pragma comment(lib,"zlibstat.lib")

bmsg g_msg;
data_v g_data;
CArray<bmsg,bmsg&> gamsg;
CArray<code_tab,code_tab&> mct;
HWND page2_wnd=NULL;

typedef int (WINAPI *UNCOMP)(unsigned char*,unsigned long*,unsigned char*,unsigned long*);
UNCOMP uncomp=NULL;
typedef int (WINAPI *INFLATEINIT32)(void *,int,const char *,int);
INFLATEINIT32 inf32=NULL;
typedef int (WINAPI *INFLATE)(void *,int);
INFLATE inf=NULL;
typedef void (WINAPI *INFLATEEND)(void *);
INFLATEEND infend=NULL;

BOOL myapp::InitInstance()
{
	mydlg mdlg;
	m_pMainWnd=&mdlg;
	mdlg.DoModal();
	return false;
};
//pump
void pump()
{
	MSG msg;
	while(::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
	{
		if(!AfxGetApp()->PumpMessage())
		{
			PostQuitMessage(0);
			return;
		}
	}
};
//建立连接字符串函数
void con_str(int code,int type,char *buf,int len)
{
	int i,j;
	memset((void*)buf,0,len);
	if(type==0)//沪市
		_snprintf_s(buf,len,len,ST1,"sh",code);
	else
		_snprintf_s(buf,len,len,ST1,"sz",code);
	i=strlen(buf);j=len-i;
	_snprintf_s((char *)&buf[i],j,j,HOST1);
	i=strlen(buf);j=len-i;
	_snprintf_s((char*)&buf[i],j,j,AGENT);
	i=strlen(buf);j=len-i;
	_snprintf_s((char*)&buf[i],j,j,ACCP1);
	i=strlen(buf);j=len-i;
	_snprintf_s((char*)&buf[i],j,j,ACCP2);
	i=strlen(buf);j=len-i;
	_snprintf_s((char*)&buf[i],j,j,ACCP3);
	i=strlen(buf);j=len-i;
	_snprintf_s((char*)&buf[i],j,j,KEEP1);
};
//获取数据的线程函数
UINT get_data(LPVOID lpvoid)
{
	int i,j,k,l;
	unsigned long L1,L2;
	UINT u;
	char ch[4096],c1[2048],c2[2048];
	SOCKET sk;
	struct sockaddr_in sa;
	HMODULE mod;
	mod=::LoadLibraryA("zlibwapi.dll");
	if(mod==NULL)
	{
		MessageBox(NULL,"加载动态库失败","msg",0);
		return 1;
	}
	uncomp=(UNCOMP)::GetProcAddress(mod,"uncompress");
	if(uncomp==NULL)
	{
		::FreeLibrary(mod);
		MessageBox(NULL,"取得函数地址失败","msg",0);
		return 1;
	}
	memset((void*)&sa,0,sizeof(sa));
	sa.sin_family=AF_INET;
	sa.sin_addr.S_un.S_addr=inet_addr(HOSTNAME);
	sa.sin_port=htons(80);
	CString str,s1,s2;
	memset((void*)&ch,0,sizeof(ch));
	for(i=0;i<1;i++)
	{
		sk=socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
		if(sk==INVALID_SOCKET)
		{
			_snprintf_s(ch,1024,1024,"create socket error %d",i+600000);
			::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
			goto end_01;
		}
		if(connect(sk,(sockaddr *)&sa,sizeof(sa))!=0)
		{
			_snprintf_s(ch,1024,1024,"connect error %d",i+600000);
			::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
			goto end_01;
		}
		con_str(i+600000,0,ch,sizeof(ch));
		if(send(sk,ch,strlen(ch),0)==SOCKET_ERROR)
		{
			_snprintf_s(ch,1024,1024,"send error %d",i+600000);
			::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
			goto end_01;
		}
		memset(ch,0,sizeof(ch));
		j=recv(sk,ch,sizeof(ch),0);
		if(j==SOCKET_ERROR)
		{
			_snprintf_s(ch,4096,4096,"recv error %d",i+600000);
			::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
			goto end_01;
		}
		memset((void*)&c1,0,sizeof(c1));k=0;
		for(k=0;k<4096;k++)
		{
			if((unsigned char)ch[k]==0xd && (unsigned char)ch[k+1]==0xa && (unsigned char)ch[k+2]==0xd && (unsigned char)ch[k+3]==0xa)
				break;
		}
		k+=4;//11
		CFile file;
		if(!file.Open("a001.ttt",CFile::modeCreate|CFile::modeReadWrite,0))
		{
			_snprintf_s(ch,4096,4096,"create file error %d",i+600000);
			::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
			goto end_01;
		}
		memcpy((void*)&c1,(void*)&ch,j);
		file.Write((void*)&c1,j);
		file.Close();
		/*for(l=j;l<4096;l++)
		{
			if(ch[l]==0)
				continue;
			memcpy((void*)&c1,(void*)&ch[l],strlen(&ch[l]));
			break;
		}*/
		memset((void*)&c2,0,sizeof(c2));
		L2=2048;L1=j-k-8;
//		j=uncomp((unsigned char*)&c2,&L2,(unsigned char*)&c1,&L1);
		_snprintf_s((char*)&c2,2048,2048,"okok! return=%d",j);
		::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&c2);
		closesocket(sk);
		Sleep(60);
	}
end_01:
	::FreeLibrary(mod);
	return 0;
};
UINT get_data1(LPVOID lpvoid)
{
	CString str,s1,s2;
	char ch[4096],c1[2048],c2[2048],*c;
	int i,j,k;
	DWORD scode;
	CInternetSession session("HttpClient");
	CHttpFile *pfile;
	for(i=0;i<80;i++)
	{
		memset((void*)&ch,0,sizeof(ch));
		_snprintf_s(ch,4096,4096,"http://hq.sinajs.cn/list=sh%d",i+600000);
		pfile=(CHttpFile*)session.OpenURL(ch);
		pfile->QueryInfoStatusCode(scode);
		if(scode==HTTP_STATUS_OK)
		{
			pfile->ReadString(str);
			memset((void*)&ch,0,sizeof(ch));
			memcpy((void*)&ch,str.GetBuffer(0),str.GetLength());
			c=strchr(ch,'"');
			if(c!=NULL)
			{
				c++;
				memset((void*)&c1,0,sizeof(c1));
				memcpy((void*)&c1,c,strlen(c));
				if(c1[0]=='"')
				{
					pfile->Close();
					delete pfile;
					Sleep(60);
					continue;
				}
				c=strchr(c1,'"');
				if(c!=NULL)
				{
					j=c-c1;c1[j]=0;c1[j+1]=0;
				}
				else
				{
					memset((void*)&c1,0,sizeof(c1));
					_snprintf_s(c1,sizeof(c1),sizeof(c1),"errooroo");
				}
			}
			else
			{
					memset((void*)&c1,0,sizeof(c1));
					_snprintf_s(c1,sizeof(c1),sizeof(c1),"errooroo111");
			}
			::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)c1);
			Sleep(60);
		}
		pfile->Close();
		delete pfile;
	}
	session.Close();
	return 0;
};
int unzip(char *source,int slen,char *dest,int flag)
{
	HMODULE mod;
	int ret;
	unsigned int have;
	z_stream strm;
//	unsigned char out[2048];
	strm.zalloc = Z_NULL; 
	strm.zfree = Z_NULL; 
	strm.opaque = Z_NULL; 
	strm.avail_in = 0; 
	strm.next_in = Z_NULL;
	mod=::LoadLibraryA("zlibwapi.dll");
	if(mod==NULL)
	{
		MessageBox(NULL,"加载动态库失败","msg",0);
		return 1;
	}
	inf32=(INFLATEINIT32)::GetProcAddress(mod,"inflateInit2_");
	if(inf32==NULL)
	{
		::FreeLibrary(mod);
		MessageBox(NULL,"取得函数地址失败","msg",0);
		return 1;
	}
	inf=(INFLATE)::GetProcAddress(mod,"inflate");
	if(inf==NULL)
	{
		::FreeLibrary(mod);
		MessageBox(NULL,"取得函数inflate地址失败","msg",0);
		return 1;
	}
	infend=(INFLATEEND)::GetProcAddress(mod,"inflateEnd");
	if(infend==NULL)
	{
		::FreeLibrary(mod);
		MessageBox(NULL,"取得函数inflateEnd地址失败","msg",0);
		return 1;
	}
	if(flag==1)
		ret = inf32((void*)&strm, 47,ZLIB_VERSION,sizeof(strm)); 
	else
		return 1;//目前只需要解压zip格式，zlib格式的暂不需要
	strm.avail_in = slen; 
	strm.next_in = (Bytef*)source;
	strm.avail_out = 2048; 
	strm.next_out = (Bytef*)dest; 
	ret = inf(&strm, Z_NO_FLUSH);
	infend(&strm); 
	::FreeLibrary(mod);
	return ret;
/*	if(flag==1)
		ret = inflateInit2_(&strm, 47); 
	else
		return 1;//目前只需要解压zip格式，zlib格式的暂不需要
	if(ret!=Z_OK)
		return ret;
	strm.avail_in = slen; 
	strm.next_in = (Bytef*)source;
	strm.avail_out = 2048; 
	strm.next_out = out; 
	ret = inflate(&strm, Z_NO_FLUSH); */
	/*
	if(ret != Z_STREAM_ERROR); 
	{
		switch (ret) { 
		case Z_NEED_DICT: 
			ret = Z_DATA_ERROR; 
		case Z_DATA_ERROR: 
		case Z_MEM_ERROR: 
			inflateEnd(&strm); 
			return ret; 
		};
	}*/
/*	inflateEnd(&strm); 
	return ret; */
};

//获取数据的线程函数
UINT get_data2(LPVOID lpvoid)
{
	int i,j,k,l;
	unsigned long L1,L2;
	UINT u;
	char ch[4096],c1[2048],c2[2048];
	SOCKET sk;
	struct sockaddr_in sa;
	memset((void*)&sa,0,sizeof(sa));
	sa.sin_family=AF_INET;
	sa.sin_addr.S_un.S_addr=inet_addr(HOSTNAME);
	sa.sin_port=htons(80);
	CString str,s1,s2;
	memset((void*)&ch,0,sizeof(ch));
	for(i=0;i<1;i++)
	{
		sk=socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
		if(sk==INVALID_SOCKET)
		{
			_snprintf_s(ch,1024,1024,"create socket error %d",i+600000);
			::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
			goto end_01;
		}
		if(connect(sk,(sockaddr *)&sa,sizeof(sa))!=0)
		{
			_snprintf_s(ch,1024,1024,"connect error %d",i+600000);
			::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
			goto end_01;
		}
		con_str(i+600000,0,ch,sizeof(ch));
		if(send(sk,ch,strlen(ch),0)==SOCKET_ERROR)
		{
			_snprintf_s(ch,1024,1024,"send error %d",i+600000);
			::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
			goto end_01;
		}
		memset(ch,0,sizeof(ch));
		j=recv(sk,ch,sizeof(ch),0);
		if(j==SOCKET_ERROR)
		{
			_snprintf_s(ch,4096,4096,"recv error %d",i+600000);
			::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
			goto end_01;
		}
		memset((void*)&c1,0,sizeof(c1));k=0;
		for(k=0;k<4096;k++)
		{
			if((unsigned char)ch[k]==0xd && (unsigned char)ch[k+1]==0xa && (unsigned char)ch[k+2]==0xd && (unsigned char)ch[k+3]==0xa)
				break;
		}
		k+=4;//11
		memcpy((void*)&c1,(void*)&ch[k],j-k);
		memset((void*)&c2,0,sizeof(c2));
		j=unzip((char*)&c1,j-k,(char*)&c2,1);
		if(j<0 || j>1)
			_snprintf_s((char*)&c2,2048,2048,"okok! return=%d",j);
		::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&c2);
		closesocket(sk);
		Sleep(60);
	}
end_01:
	return 0;
};

