#include"../pg001/stdafx.h"
#include"myrecord.h"

#define dbfile			"ggupiao1.accdb"

int whichtab=0;

//��������ڴ����Ļ�����Ϣ�ṹ���У���Ӧ��һ��
_declspec(dllexport) CArray<bmsg,bmsg&>	dlmsg;
//��������ڴ�������ݽṹ���У���Ӧ�����
_declspec(dllexport) CArray<data_v,data_v&> dldata;


//����������ݿ�洢�µĻ�����Ϣ�ĺ���
_declspec(dllexport) int WINAPI putmsg(void *res); 
//����Ĵ����ݿ�����Ļ�����Ϣ�ĺ���
_declspec(dllexport) int WINAPI getmsg(void *res);
//����������ݿ�洢���ݵĺ���
_declspec(dllexport) int WINAPI putdata(void *res);
//����Ĵ����ݿ��ȡ���ݵĺ���
_declspec(dllexport) int WINAPI getdata(void *res);
//������ֶ�ɾ��������Ϣ���ݵĺ���
_declspec(dllexport) int WINAPI delmsg(void *res);
//�����ɾ�����ݵĺ���
_declspec(dllexport) int WINAPI deldata(void *res);

//�����ģ���ڲ�ʹ�õĺ������������ݿ����ӹؼ���
BOOL crt_conn(CString &str);
















