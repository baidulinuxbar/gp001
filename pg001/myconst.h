#define ID_MY_CONST_THIS

#define WM_TEST_MESSAGE			WM_USER+0x133

//60.215.128.238
//http://hq.sinajs.cn/list=sh600303
/*报文格式：
上证：sh；深证：sz
0: 	股票名称	
1：	开盘价
2：	昨收盘
3：	当前价
4：	今最高
5：	今最低
6：	买入价
7：	卖出价
8：	成交量
9：	成交额
10：	买一
11：	买一价
12：	买二
13：	买二价
14：	买三
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

