#define ID_MYDATA_STRUCT
#ifndef BYTE
#define BYTE			unsigned char
#endif

typedef struct BASE_MSG_DATA
{
	int		gid;
	BYTE	gtype;					//股市类型：深、沪
	int		code;					//股票代码
	char	name[60];				//股票名称
	BYTE	gwatch;					//监控类型： 0：三重顶 
	int		gtimes;					//数据获取周期
	double		gmax;					//波峰
	double		gstd;					//基准值
	BYTE	gcycle;					//三重顶形成的最小周期（索引）
	BYTE	gdecay;					//三重顶波谷的衰减率（索引）
}bmsg;
typedef struct DATA_VALUE
{
	int	gid;
	CTime	tm;						//长型日期
	double	gv;						//数据值
}data_v;

const int cycle[6]={3,4,5,10,15,20};	//监控的最短周期
const double decay[6]={0.8,0.7,0.6,0.5,0.4,0.3};		//波谷的衰减率

typedef int (WINAPI *PUTMSG)(void *);
typedef int (WINAPI *GETMSG)(void *);
typedef int (WINAPI *PUTDATA)(void *);
typedef int (WINAPI *GETDATA)(void *);
typedef int (WINAPI *DELMSG)(void *);
typedef int (WINAPI *DELDATA)(void *);
