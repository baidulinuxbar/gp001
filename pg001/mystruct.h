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
//2015-6-18添加，对应表格： code_tab
typedef struct CODE_TAB
{
	CTime	gp_date;					//已获取记录的日期
	int		gp_count;					//当日获取的记录数量
}code_tab;
//2015-6-18添加，对应表格：data_tab
typedef struct DATA_TAB
{
	int		gp_code;					//股票代码
	CTime	gp_date;					//日期
	double	pre;						//昨日收盘价
	double	beg;						//今日开盘价
	double	cur;						//当前价
	double	cur_max;					//今日最高
	double	cur_min;					//今日最低
	int		total_cnt;					//成交量
	int		total_pri;					//成交额
	char	gp_name[60];				//股票名称
	BYTE	gp_type;					//机构 0：沪市  1：深市
}data_tab;


const int cycle[6]={3,4,5,10,15,20};	//监控的最短周期
const double decay[6]={0.8,0.7,0.6,0.5,0.4,0.3};		//波谷的衰减率

typedef int (WINAPI *PUTMSG)(void *);
typedef int (WINAPI *GETMSG)(void *);
typedef int (WINAPI *PUTDATA)(void *);
typedef int (WINAPI *GETDATA)(void *);
typedef int (WINAPI *DELMSG)(void *);
typedef int (WINAPI *DELDATA)(void *);
