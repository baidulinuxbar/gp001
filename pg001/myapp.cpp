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
data_tab dtab;
CArray<bmsg,bmsg&> gamsg;
CArray<code_tab,code_tab&> mct;
CArray<data_tab,data_tab&> mdt;
HWND page2_wnd=NULL;
HWND page1_wnd=NULL;
HMODULE mod=NULL;
/*下列结构是判断是否重复获取记录的依据，该结构在程序启动时初始化，并且在获取了新记录后进行更新
f_ct存储了数据库中最早的记录日期，l_ct存储了最新记录的日期
*/
code_tab f_ct,l_ct;
/*下列标志用于即时查询时自动更新的标志，=0表示停止更新，=1表示持续更新
该标志的设置是通过即时查询页面的“持续更新”单选按钮设置的，
但是更新停止不仅可由单选按钮来设置，也可通过页面的切换来自动停止。
*/
int keep_acc=0;
/*下列标志用于标识即时查询线程是否运行，=1正在运行，=0停止
该标志的设置目的是：当持续查询一个股票时，用户再查询另外一只股票时，不必关闭原来的查询线程，
而仅仅是更换了股票代码就可改变查询。但是，首次运行时通过查看该标志，启动线程。
该线程的关闭没有直接的操作控件，当没有设置上面的持续查询标志时，查询一次完成即退出。
当设置了持续查询标志时，将循环查询，直至持续查询标志设置为停止。
*/
int thd_running=0;
/*下列标志用于即时查询时存储待查询的股票代码*/
int imm_code=0;

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
int strfmt(CString str,data_tab &dt);
time_t str_time(char *chr);

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
	mod=NULL;
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
/*2015-6-21添加，该线程函数用于即时查询时获取远程记录数据*/
UINT get_data_imm(LPVOID lpvoid)
{
	if(thd_running)
	{
		MessageBox(NULL,"aaaa","bb",0);
		return 0;
	}
	int errtimes,i,j,k;
	CString str;
	thd_running=1;		//该标志必须由线程自己设置,主进程只获取其状态。
	if(mod!=NULL)		//记录获取线程在运行
	{
		MessageBox(NULL,"aaaa11","bb",0);
		goto end_03;
	}
	char ch[4096],c1[2048],c2[2048];
	SOCKET sk;
	struct sockaddr_in sa;
	mod=::LoadLibraryA("zlibwapi.dll");
	if(mod==NULL)
	{//这里可以使用消息框，因为该操作是人为的，不会是自动运行的。
		MessageBox(NULL,"加载链接库失败！","来自线程",0);
		goto end_03;
	}
	inf32=(INFLATEINIT32)::GetProcAddress(mod,"inflateInit2_");
	if(inf32==NULL)
	{
		MessageBox(NULL,"取得函数inf32地址失败！","来自线程",0);
		goto end_02;
	}
	inf=(INFLATE)::GetProcAddress(mod,"inflate");
	if(inf==NULL)
	{
		MessageBox(NULL,"取得函数inf地址失败！","来自线程",0);
		goto end_02;
	}
	infend=(INFLATEEND)::GetProcAddress(mod,"inflateEnd");
	if(infend==NULL)
	{
		MessageBox(NULL,"取得函数infend地址失败！","来自线程",0);
		goto end_02;
	}
	errtimes=6;
imm_01:
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
			Sleep(1000);
			goto imm_01;
		}
		MessageBox(NULL,"建立套接字失败！","来自线程",0);
		goto end_02;
	}
	if(connect(sk,(sockaddr *)&sa,sizeof(sa))!=0)
	{
		errtimes--;
		if(errtimes>0)
		{
			closesocket(sk);
			Sleep(1000);
			goto imm_01;
		}
		MessageBox(NULL,"建立连接失败！","来自线程",0);
		goto end_02;
	}
	if(imm_code>600000)//沪市
		i=0;
	else
		i=1;
	con_str(imm_code,i,ch,sizeof(ch));
	if(send(sk,ch,strlen(ch),0)==SOCKET_ERROR)
	{
		errtimes--;
		if(errtimes>0)
		{
			closesocket(sk);
			i--;
			Sleep(1000);
			goto imm_01;
		}
		MessageBox(NULL,"数据包发送失败！","来自线程",0);
		goto end_02;
	}
	memset(ch,0,sizeof(ch));
	j=recv(sk,ch,sizeof(ch),0);
	if(j==SOCKET_ERROR)
	{
		errtimes--;
		if(errtimes>0)
		{
			closesocket(sk);
			Sleep(1000);
			goto imm_01;
		}
		MessageBox(NULL,"接收数据包失败！","来自线程",0);
		goto end_02;
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
		MessageBox(NULL,"没有数据","来自线程",0);
		goto end_02;
	}
	closesocket(sk);
	str.Format("%s",c2);
	if(strfmt(str,dtab))
	{
		MessageBox(NULL,"时间字符串格式化错误","来自线程",0);
		goto end_02;
	}
	::SendMessageA(page1_wnd,WM_IMM_MESSAGE,0,0);
	Sleep(2000);
	if(keep_acc==1)
		goto imm_01;
end_02:
	::FreeLibrary(mod);
end_03:
	thd_running=0;
	mod=NULL;
	return 0;
};
//日期字符串查找并转换函数
time_t str_time(char *chr)
{
	int y,m,d,h,M,s,i,j,k;
	time_t tt;
	CString str,str1;
	char ch[20],*c;
	str.Format("%s",chr);
	i=str.ReverseFind(',');
	if(i<0)
		return 0;
	str1=str.Left(i);
	str=str1;
	i=str.ReverseFind(',');
	if(i<0)
		return 0;
	str1=str.Right(str.GetLength()-i-1);
	memset((void*)&ch,0,sizeof(ch));
	memcpy(ch,str1.GetBuffer(0),str1.GetLength());
	c=ch;
	h=atoi(c);c+=3;
	M=atoi(c);c+=3;
	s=atoi(c);
	str1=str.Left(i);
	str=str1;
	i=str.ReverseFind(',');
	if(i<0)
		return 0;
	str1=str.Right(str.GetLength()-i-1);
	memset((void*)&ch,0,sizeof(ch));
	memcpy(ch,str1.GetBuffer(0),str1.GetLength());
	c=ch;
	y=atoi(c);c+=5;
	m=atoi(c);c+=3;
	d=atoi(c);
	CTime tm(y,m,d,h,M,s);
	tt=tm.GetTime();
	return tt;
};
//字符串格式化函数
int strfmt(CString str,data_tab &dt)
{
	int i,j,k;
	time_t tt;
	char ch[2048],*c1,*c2,ch1[200];
	memset((void*)&dt,0,sizeof(dt));
	memset((void*)&ch,0,sizeof(ch));
	memcpy(ch,str.GetBuffer(0),str.GetLength());
	c1=ch;c1+=13;//得到股票代码
	memset((void*)&ch1,0,sizeof(ch1));
	memcpy(ch1,c1,6);
	dt.gp_code=atoi(ch1);//保存代码
	if(dt.gp_code>=600000)
		dt.gp_type=0;
	else
		dt.gp_type=1;
	c2=strchr(c1,'"');
	if(c2==NULL)
		return 1;
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(dt.gp_name,c1,i);//保存股票名称
	memset((void*)&ch1,0,sizeof(ch1));
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(ch1,c1,i);
	dt.beg=atof(ch1);//开盘价
	memset((void*)&ch1,0,sizeof(ch1));
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(ch1,c1,i);
	dt.pre=atof(c1);//昨日收盘价
	memset((void*)&ch1,0,sizeof(ch1));
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(ch1,c1,i);
	dt.cur=atof(c1);//当前价
	memset((void*)&ch1,0,sizeof(ch1));
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(ch1,c1,i);
	dt.cur_max=atof(c1);//今日最高价
	memset((void*)&ch1,0,sizeof(ch1));
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(ch1,c1,i);
	dt.cur_min=atof(c1);//今日最低价
	memset((void*)&ch1,0,sizeof(ch1));
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	c1=c2+1;			//跳过买入价
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	c1=c2+1;			//跳过卖出价
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(dt.total_cnt,c1,i);	//成交量
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(dt.total_pri,c1,i);	//成交额
	i=str.ReverseFind(',');
	tt=str_time(ch);
	if(tt==0)
		return 1;
	CTime tmm(tt);
	dt.gp_date=tmm;
	return 0;	
};















