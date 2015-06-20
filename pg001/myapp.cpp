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
HMODULE mod=NULL;
//使用二维数组定义待扫描的股票代码，当一维小于4时为沪市，否则为深市
int gpcode[8][2]={
	{600000,600500},
	{600500,601000},
	{601000,601500},
	{601500,602000},
	{0,500},
	{500,1000},
	{2000,2600},
	{300000,300190},
};

/*typedef int (WINAPI *UNCOMP)(unsigned char*,unsigned long*,unsigned char*,unsigned long*);
UNCOMP uncomp=NULL;*/
typedef int (WINAPI *INFLATEINIT32)(void *,int,const char *,int);
INFLATEINIT32 inf32=NULL;
typedef int (WINAPI *INFLATE)(void *,int);
INFLATE inf=NULL;
typedef void (WINAPI *INFLATEEND)(void *);
INFLATEEND infend=NULL;

int unzip(char *source,int slen,char *dest,int flag);

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
	int i,j,k,v1,v2,errtimes;
	CString str,s1,s2;
	char ch[4096],c1[2048],c2[2048];
	SOCKET sk;
	struct sockaddr_in sa;
	mod=::LoadLibraryA("zlibwapi.dll");
	if(mod==NULL)
	{
		_snprintf_s(ch,1024,1024,"load library error");
		::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
		return 1;
	}
	inf32=(INFLATEINIT32)::GetProcAddress(mod,"inflateInit2_");
	if(inf32==NULL)
	{
		_snprintf_s(ch,1024,1024,"get proc address inf32 error");
		::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
		goto end_01;
	}
	inf=(INFLATE)::GetProcAddress(mod,"inflate");
	if(inf==NULL)
	{
		_snprintf_s(ch,1024,1024,"get proc address inf error");
		::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
		goto end_01;
	}
	infend=(INFLATEEND)::GetProcAddress(mod,"inflateEnd");
	if(infend==NULL)
	{
		_snprintf_s(ch,1024,1024,"get proc address infend error");
		::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
		goto end_01;
	}
	errtimes=6;//容错次数
	for(v1=0;v1<8;v1++)
	{
		if(v1<4)
			v2=0; //沪市
		else
			v2=1; //深市
		for(i=gpcode[v1][0];i<gpcode[v1][1];i++)
		{
			memset((void*)&ch,0,sizeof(ch));
			memset((void*)&sa,0,sizeof(sa));
			sa.sin_family=AF_INET;
			sa.sin_addr.S_un.S_addr=inet_addr(HOSTNAME);
			sa.sin_port=htons(80);
			sk=socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
			if(sk==INVALID_SOCKET)
			{
				errtimes--;
				if(errtimes>0)
				{
					i--;
					Sleep(1000);
					continue;
				}
				_snprintf_s(ch,1024,1024,"create socket error %d",i);
				::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
				goto end_01;
			}
			if(connect(sk,(sockaddr *)&sa,sizeof(sa))!=0)
			{
				errtimes--;
				if(errtimes>0)
				{
					closesocket(sk);
					i--;
					Sleep(1000);
					continue;
				}
				_snprintf_s(ch,1024,1024,"connect error %d",i);
				::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
				goto end_01;
			}
			con_str(i,v2,ch,sizeof(ch));
			if(send(sk,ch,strlen(ch),0)==SOCKET_ERROR)
			{
				errtimes--;
				if(errtimes>0)
				{
					closesocket(sk);
					i--;
					Sleep(1000);
					continue;
				}
				_snprintf_s(ch,1024,1024,"send error %d",i);
				::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
				goto end_01;
			}
			memset(ch,0,sizeof(ch));
			j=recv(sk,ch,sizeof(ch),0);
			if(j==SOCKET_ERROR)
			{
				errtimes--;
				if(errtimes>0)
				{
					closesocket(sk);
					i--;
					Sleep(1000);
					continue;
				}
				_snprintf_s(ch,4096,4096,"recv error %d",i);
				::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&ch);
				goto end_01;
			}
			memset((void*)&c1,0,sizeof(c1));
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
			{
				closesocket(sk);
				if(j==-3)//按照代码遍历，该代码没有股票
				{
					Sleep(60);
					continue;
				}
				else
				{
					_snprintf_s((char*)&c2,2048,2048,"data uncompress error:%d",j);
					::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&c2);
					goto end_01;
				}
			}
			::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&c2);
			closesocket(sk);
			Sleep(60);
		}
		Sleep(3000);
		::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,NULL);
		Sleep(5000);
	}
end_01:
	::FreeLibrary(mod);
	return 0;
};
int unzip(char *source,int slen,char *dest,int flag)
{
	int ret;
	z_stream strm;
	strm.zalloc = Z_NULL; 
	strm.zfree = Z_NULL; 
	strm.opaque = Z_NULL; 
	strm.avail_in = 0; 
	strm.next_in = Z_NULL;
	if(flag==1)
		ret = inf32((void*)&strm, 47,ZLIB_VERSION,sizeof(strm)); 
	else
		return 100;//目前只需要解压zip格式，zlib格式的暂不需要
	strm.avail_in = slen; 
	strm.next_in = (Bytef*)source;
	strm.avail_out = 2048; 
	strm.next_out = (Bytef*)dest; 
	ret = inf(&strm, Z_NO_FLUSH);
	infend(&strm); 
	return ret;
};


