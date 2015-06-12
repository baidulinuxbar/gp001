#ifndef ID_MYDATA_STRUCT
#include"..\pg001\mystruct.h"
#endif

class myrecord:public CRecordset
{
public:
	BASE_MSG_DATA  bsd;
	DATA_VALUE	dv;
	myrecord(CDatabase *db);
	CString GetDefaultSQL();
	void DoFieldExchange(CFieldExchange *pdx);
};


