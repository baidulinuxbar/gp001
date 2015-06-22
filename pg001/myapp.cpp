#include"stdafx.h"
#include"resource.h"
#include"mydlg.h"
#include"myapp.h"
#include"mystruct.h"
#include"myconst.h"
#include"zconf.h"
#include"zlib.h"

//#pragma comment(lib,"zlibstat.lib")

code_tab ctab;							//��Ϣ��Ľṹ��
data_tab dtab;							//���ݱ�Ľṹ��
CArray<code_tab,code_tab&> mct;			//��Ϣ��Ľṹ����
CArray<data_tab,data_tab&> mdt;			//���ݱ�Ľṹ����
HWND page2_wnd=NULL;					//����2������ץȡ���棩�Ĵ��ھ������Ϣ����ʹ��
HWND page1_wnd=NULL;					//����1����ʱ��ѯ���棩�Ĵ��ھ������Ϣ����ʹ��
HMODULE mod=NULL;						//zlib��ר�õ�ģ����


/*���нṹ���ж��Ƿ��ظ���ȡ��¼�����ݣ��ýṹ�ڳ�������ʱ��ʼ���������ڻ�ȡ���¼�¼����и���
f_ct�洢�����ݿ�������ļ�¼���ڣ�l_ct�洢�����¼�¼������
*/
code_tab f_ct,l_ct;
/*���б�־���ڼ�ʱ��ѯʱ�Զ����µı�־��=0��ʾֹͣ���£�=1��ʾ��������
�ñ�־��������ͨ����ʱ��ѯҳ��ġ��������¡���ѡ��ť���õģ�
���Ǹ���ֹͣ�������ɵ�ѡ��ť�����ã�Ҳ��ͨ��ҳ����л����Զ�ֹͣ��
*/
int keep_acc=0;
/*���б�־���ڱ�ʶ��ʱ��ѯ�߳��Ƿ����У�=1�������У�=0ֹͣ
�ñ�־������Ŀ���ǣ���������ѯһ����Ʊʱ���û��ٲ�ѯ����һֻ��Ʊʱ�����عر�ԭ���Ĳ�ѯ�̣߳�
�������Ǹ����˹�Ʊ����Ϳɸı��ѯ�����ǣ��״�����ʱͨ���鿴�ñ�־�������̡߳�
���̵߳Ĺر�û��ֱ�ӵĲ����ؼ�����û����������ĳ�����ѯ��־ʱ����ѯһ����ɼ��˳���
�������˳�����ѯ��־ʱ����ѭ����ѯ��ֱ��������ѯ��־����Ϊֹͣ��
*/
int thd_running=0;
/*���б�־���ڼ�ʱ��ѯʱ�洢����ѯ�Ĺ�Ʊ����*/
int imm_code=0;

//ʹ�ö�ά���鶨���ɨ��Ĺ�Ʊ���룬��һάС��4ʱΪ���У�����Ϊ����
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



typedef int (WINAPI *INFLATEINIT32)(void *,int,const char *,int);
INFLATEINIT32 inf32=NULL;
typedef int (WINAPI *INFLATE)(void *,int);
INFLATE inf=NULL;
typedef void (WINAPI *INFLATEEND)(void *);
INFLATEEND infend=NULL;

//ǰ�������ĺ���
int unzip(char *source,int slen,char *dest,int flag);  //gzip�Ľ�ѹʵ�ֺ���
int strfmt(CString str,data_tab &dt);				   //���ݰ��ַ�����ʽ��Ϊ���ݱ�ṹ
time_t str_time(char *chr);							   //�ַ�����ʽ������ת��ΪCTime�ṹ��
//���ݿ�洢����
int put_db();

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
//���������ַ�������
void con_str(int code,int type,char *buf,int len)
{
	int i,j;
	memset((void*)buf,0,len);
	if(type==0)//����
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
//��ȡ���ݵ��̺߳���
UINT get_data(LPVOID lpvoid)
{
	int x=0;
	int y1,y2;
	int i,j,k,v1,v2,errtimes;
	CString str,s1,s2;
	char ch[4096],c1[2048],c2[2048];
	SOCKET sk;
	struct sockaddr_in sa;
	mdt.RemoveAll();
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
	errtimes=10;//�ݴ����
	for(v1=0;v1<8;v1++)
	{
		if(v1<4)
			v2=0; //����
		else
			v2=1; //����
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
				if(j==-3)//���մ���������ô���û�й�Ʊ
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
			closesocket(sk);
			s1.Format("%s",c2);
			if(strfmt(s1,dtab))
			{
				_snprintf_s((char*)&c2,2048,2048,"format error:%d",j);
				::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&c2);
				goto end_01;
			}
			if(x==0)
			{//�״λ�ȡ����Ҫ�����ݿ��������������ݵ����ڱȽϣ�����ȡ���ݵ����ڱ����ݿ����򱣴�
				//�����˳�
				x=1;
				y1=(int)(ctab.gp_edate.GetTime()/86400); //ȡ������
				y2=(int)(dtab.gp_date.GetTime()/86400);
				if(y1>=y2) //���豣��
				{
					_snprintf_s((char*)&c2,2048,2048,"��ǰ��ȡ�������Ѿ����������ݿ��У���ȡ����ֹͣ");
					::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&c2);
					goto end_01;
				}
				y2=dtab.gp_date.GetHour();
				if(y2>=9 && y2<15)
				{//���ڿ����ڼ䲻�ܱ���
					_snprintf_s((char*)&c2,2048,2048,"��ǰ�������ڿ��У���ȡ����ֹͣ");
					::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&c2);
					goto end_01;
				}
			}
			mdt.Add(dtab);
			::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&c2);
			Sleep(60);
		}
		Sleep(3000);
		::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,NULL);
		Sleep(5000);
	}
	j=put_db();
	switch(j)
	{
	case 0://�ɹ�
		_snprintf_s((char*)&c2,2048,2048,"���ݳɹ����������ݿ�");
		break;
	case 1://�������ӿ�ʧ��
		_snprintf_s((char*)&c2,2048,2048,"�������ݿ���ʵ����ӿ�ʧ��");
		break;
	case 2://ȡ�õ�ַʧ��
		_snprintf_s((char*)&c2,2048,2048,"ȡ�����ݿ���ʺ���putdata�ĵ�ַʧ��");
		break;
	case 3://
		_snprintf_s((char*)&c2,2048,2048,"ȡ�����ݿ���ʺ���putmsg�ĵ�ַʧ��");
		break;
	case 4://
		_snprintf_s((char*)&c2,2048,2048,"���ݿ�洢���ݱ�ʧ��");
		break;
	case 5://
		_snprintf_s((char*)&c2,2048,2048,"���ݿ�洢��Ϣ��ʧ��");
		break;
	};
	::SendMessageA(page2_wnd,WM_TEST_MESSAGE,0,(LPARAM)&c2);
	mdt.RemoveAll();
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
		return 100;//Ŀǰֻ��Ҫ��ѹzip��ʽ��zlib��ʽ���ݲ���Ҫ
	strm.avail_in = slen; 
	strm.next_in = (Bytef*)source;
	strm.avail_out = 2048; 
	strm.next_out = (Bytef*)dest; 
	ret = inf(&strm, Z_NO_FLUSH);
	infend(&strm); 
	return ret;
};
/*2015-6-21��ӣ����̺߳������ڼ�ʱ��ѯʱ��ȡԶ�̼�¼����*/
UINT get_data_imm(LPVOID lpvoid)
{
	if(thd_running)
	{
		MessageBox(NULL,"aaaa","bb",0);
		return 0;
	}
	int errtimes,i,j,k;
	CString str;
	thd_running=1;		//�ñ�־�������߳��Լ�����,������ֻ��ȡ��״̬��
	if(mod!=NULL)		//��¼��ȡ�߳�������
	{
		MessageBox(NULL,"aaaa11","bb",0);
		goto end_03;
	}
	char ch[4096],c1[2048],c2[2048];
	SOCKET sk;
	struct sockaddr_in sa;
	mod=::LoadLibraryA("zlibwapi.dll");
	if(mod==NULL)
	{//�������ʹ����Ϣ����Ϊ�ò�������Ϊ�ģ��������Զ����еġ�
		MessageBox(NULL,"�������ӿ�ʧ�ܣ�","�����߳�",0);
		goto end_03;
	}
	inf32=(INFLATEINIT32)::GetProcAddress(mod,"inflateInit2_");
	if(inf32==NULL)
	{
		MessageBox(NULL,"ȡ�ú���inf32��ַʧ�ܣ�","�����߳�",0);
		goto end_02;
	}
	inf=(INFLATE)::GetProcAddress(mod,"inflate");
	if(inf==NULL)
	{
		MessageBox(NULL,"ȡ�ú���inf��ַʧ�ܣ�","�����߳�",0);
		goto end_02;
	}
	infend=(INFLATEEND)::GetProcAddress(mod,"inflateEnd");
	if(infend==NULL)
	{
		MessageBox(NULL,"ȡ�ú���infend��ַʧ�ܣ�","�����߳�",0);
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
		MessageBox(NULL,"�����׽���ʧ�ܣ�","�����߳�",0);
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
		MessageBox(NULL,"��������ʧ�ܣ�","�����߳�",0);
		goto end_02;
	}
	if(imm_code>600000)//����
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
		MessageBox(NULL,"���ݰ�����ʧ�ܣ�","�����߳�",0);
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
		MessageBox(NULL,"�������ݰ�ʧ�ܣ�","�����߳�",0);
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
		MessageBox(NULL,"û������","�����߳�",0);
		goto end_02;
	}
	closesocket(sk);
	str.Format("%s",c2);
	if(strfmt(str,dtab))
	{
		MessageBox(NULL,"ʱ���ַ�����ʽ������","�����߳�",0);
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
//�����ַ������Ҳ�ת������
time_t str_time(char *chr)
{
	int y,m,d,h,M,s,i;
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
//�ַ�����ʽ������
int strfmt(CString str,data_tab &dt)
{
	int i;
	time_t tt;
	char ch[2048],*c1,*c2,ch1[200];
	memset((void*)&dt,0,sizeof(dt));
	memset((void*)&ch,0,sizeof(ch));
	memcpy(ch,str.GetBuffer(0),str.GetLength());
	c1=ch;c1+=13;//�õ���Ʊ����
	memset((void*)&ch1,0,sizeof(ch1));
	memcpy(ch1,c1,6);
	dt.gp_code=atoi(ch1);//�������
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
	memcpy(dt.gp_name,c1,i);//�����Ʊ����
	memset((void*)&ch1,0,sizeof(ch1));
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(ch1,c1,i);
	dt.beg=atof(ch1);//���̼�
	memset((void*)&ch1,0,sizeof(ch1));
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(ch1,c1,i);
	dt.pre=atof(c1);//�������̼�
	memset((void*)&ch1,0,sizeof(ch1));
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(ch1,c1,i);
	dt.cur=atof(c1);//��ǰ��
	memset((void*)&ch1,0,sizeof(ch1));
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(ch1,c1,i);
	dt.cur_max=atof(c1);//������߼�
	memset((void*)&ch1,0,sizeof(ch1));
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(ch1,c1,i);
	dt.cur_min=atof(c1);//������ͼ�
	memset((void*)&ch1,0,sizeof(ch1));
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	c1=c2+1;			//���������
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	c1=c2+1;			//����������
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(dt.total_cnt,c1,i);	//�ɽ���
	c1=c2+1;
	c2=strchr(c1,',');
	if(c2==NULL)
		return 1;
	i=c2-c1;
	memcpy(dt.total_pri,c1,i);	//�ɽ���
	i=str.ReverseFind(',');
	tt=str_time(ch);
	if(tt==0)
		return 1;
	CTime tmm(tt);
	dt.gp_date=tmm;
	return 0;	
};
//���ݿ�洢����
int put_db()
{
	HMODULE mod1;
	PUTDATA putdata=NULL;
	PUTMSG  putmsg=NULL;
	int rt=0;
	mod1=LoadLibrary("gpdll.dll");
	if(mod1==NULL)
		return 1;			//load library error
	putdata=(PUTDATA)::GetProcAddress(mod1,"put_dt");
	if(putdata==NULL)
	{
		rt=2;				//get procaddress error
		goto end_04;
	}
	putmsg=(PUTMSG)::GetProcAddress(mod1,"put_ct");
	if(putmsg==NULL)
	{
		rt=3;				//get procaddress error
		goto end_04;
	}
	if(putdata((void*)&mdt))
	{
		rt=4;
		goto end_04;
	}
	dtab=mdt.GetAt(0);
	if(ctab.gp_count<1) //�������У����ݿ�����������
	{
		ctab.gp_date=dtab.gp_date;
		ctab.gp_count=mdt.GetCount();
	}
	ctab.gp_ecount=mdt.GetCount();
	ctab.gp_edate=dtab.gp_date;
	if(putmsg((void*)&ctab))
	{
		rt=5;
		goto end_04;
	}
	rt=0;
end_04:
	::FreeLibrary(mod1);
	return rt;
};














