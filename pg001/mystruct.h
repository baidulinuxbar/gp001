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
	double	gmax;					//����
	double	gstd;					//��׼ֵ
}bmsg;
typedef struct DATA_VALUE
{
	int	gid;
	CTime	tm;						//��������
	double	gv;						//����ֵ
}data_v;

