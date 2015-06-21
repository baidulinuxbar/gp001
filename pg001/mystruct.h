#define ID_MYDATA_STRUCT
#ifndef BYTE
#define BYTE			unsigned char
#endif

typedef struct BASE_MSG_DATA
{
	int		gid;
	BYTE	gtype;					//�������ͣ����
	int		code;					//��Ʊ����
	char	name[60];				//��Ʊ����
	BYTE	gwatch;					//������ͣ� 0�����ض� 
	int		gtimes;					//���ݻ�ȡ����
	double		gmax;					//����
	double		gstd;					//��׼ֵ
	BYTE	gcycle;					//���ض��γɵ���С���ڣ�������
	BYTE	gdecay;					//���ض����ȵ�˥���ʣ�������
}bmsg;
typedef struct DATA_VALUE
{
	int	gid;
	CTime	tm;						//��������
	double	gv;						//����ֵ
}data_v;
//2015-6-18��ӣ���Ӧ��� code_tab
typedef struct CODE_TAB
{
	CTime	gp_date;					//�ѻ�ȡ��¼������
	int		gp_count;					//���ջ�ȡ�ļ�¼����
}code_tab;
//2015-6-18��ӣ���Ӧ���data_tab
typedef struct DATA_TAB
{
	int		gp_code;					//��Ʊ����
	CTime	gp_date;					//����
	double	pre;						//�������̼�
	double	beg;						//���տ��̼�
	double	cur;						//��ǰ��
	double	cur_max;					//�������
	double	cur_min;					//�������
	int		total_cnt;					//�ɽ���
	int		total_pri;					//�ɽ���
	char	gp_name[60];				//��Ʊ����
	BYTE	gp_type;					//���� 0������  1������
}data_tab;


const int cycle[6]={3,4,5,10,15,20};	//��ص��������
const double decay[6]={0.8,0.7,0.6,0.5,0.4,0.3};		//���ȵ�˥����

typedef int (WINAPI *PUTMSG)(void *);
typedef int (WINAPI *GETMSG)(void *);
typedef int (WINAPI *PUTDATA)(void *);
typedef int (WINAPI *GETDATA)(void *);
typedef int (WINAPI *DELMSG)(void *);
typedef int (WINAPI *DELDATA)(void *);
