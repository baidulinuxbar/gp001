#define ID_MY_CONST_THIS

#define WM_TEST_MESSAGE			WM_USER+0x133

//60.215.128.238
//http://hq.sinajs.cn/list=sh600303
/*���ĸ�ʽ��
��֤��sh����֤��sz
0: 	��Ʊ����	
1��	���̼�
2��	������
3��	��ǰ��
4��	�����
5��	�����
6��	�����
7��	������
8��	�ɽ���
9��	�ɽ���
10��	��һ
11��	��һ��
12��	���
13��	�����
14��	����
*/
//#define ST1		"GET /list=sh600303 HTTP/1.1\r\n"
#define HOSTNAME	"60.215.128.238"	
#define ST1			"GET /list=%s%06d HTTP/1.1\r\n"
#define HOST1	"Host: hq.sinajs.cn\r\n"
#define AGENT	"User-Agent: Mozilla/5.0 (Windows NT 6.1; rv:29.0) Gecko/20100101 Firefox/29.0\r\n"
#define ACCP1	"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
#define ACCP2	"Accept-Language: zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3\r\n"
#define ACCP3	"Accept-Encoding: gzip, deflate\r\n"
#define KEEP1	"Connection: keep-alive\r\n\r\n"

