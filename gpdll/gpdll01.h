#include"../pg001/stdafx.h"
#include"myrecord.h"

#define dbfile			"ggupiao1.accdb"

int whichtab=0;

//定义的用于传出的基本信息结构队列（对应表一）
_declspec(dllexport) CArray<bmsg,bmsg&>	dlmsg;
//定义的用于传入的数据结构队列（对应表二）
_declspec(dllexport) CArray<data_v,data_v&> dldata;


//定义的向数据库存储新的基本信息的函数
_declspec(dllexport) int WINAPI putmsg(void *res); 
//定义的从数据库读出的基本信息的函数
_declspec(dllexport) int WINAPI getmsg(void *res);
//定义的向数据库存储数据的函数
_declspec(dllexport) int WINAPI putdata(void *res);
//定义的从数据库读取数据的函数
_declspec(dllexport) int WINAPI getdata(void *res);
//定义的手动删除基本信息数据的函数
_declspec(dllexport) int WINAPI delmsg(void *res);
//定义的删除数据的函数
_declspec(dllexport) int WINAPI deldata(void *res);

//定义的模块内部使用的函数，建立数据库连接关键字
BOOL crt_conn(CString &str);
















