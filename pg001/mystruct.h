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
	double	gmax;					//波峰
	double	gstd;					//基准值
}bmsg;
typedef struct DATA_VALUE
{
	int	gid;
	CTime	tm;						//长型日期
	double	gv;						//数据值
}data_v;

