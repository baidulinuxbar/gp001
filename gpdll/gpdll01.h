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
/*------------------------2015-6-18����µı��Ĵ�����------------------------------*/
//��������ڴ����ı����Ľṹ����
_declspec(dllexport) CArray<code_tab,code_tab&> dlct;
//��������ڴ��������Ľṹ����
_declspec(dllexport) CArray<data_tab,data_tab&> dldt;
//����ı��3 code_tab�Ĵ洢�������ú��������ڼ�¼����������
_declspec(dllexport) int WINAPI put_ct(void *res);
//����ı��4 data_tab�Ĵ洢�������ú���Ҳ���ڼ�¼����������
_declspec(dllexport) int WINAPI put_dt(void *res);
//����ı��3�Ķ�ȡ����
_declspec(dllexport) int WINAPI get_ct(void *res);
//����ı��4�Ķ�ȡ����
_declspec(dllexport) int WINAPI get_dt(void *res);



//�����ģ���ڲ�ʹ�õĺ������������ݿ����ӹؼ���
BOOL crt_conn(CString &str);
















