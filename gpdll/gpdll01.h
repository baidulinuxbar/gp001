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
/*------------------------2015-6-18添加新的表格的处理函数------------------------------*/
//定义的用于传出的表三的结构队列
_declspec(dllexport) CArray<code_tab,code_tab&> dlct;
//定义的用于传出表三的结构队列
_declspec(dllexport) CArray<data_tab,data_tab&> dldt;
//定义的表格3 code_tab的存储函数，该函数将用于记录的批量处理。
_declspec(dllexport) int WINAPI put_ct(void *res);
//定义的表格4 data_tab的存储函数，该函数也用于记录的批量处理
_declspec(dllexport) int WINAPI put_dt(void *res);
//定义的表格3的读取函数
_declspec(dllexport) int WINAPI get_ct(void *res);
//定义的表格4的读取函数
_declspec(dllexport) int WINAPI get_dt(void *res);



//定义的模块内部使用的函数，建立数据库连接关键字
BOOL crt_conn(CString &str);
















