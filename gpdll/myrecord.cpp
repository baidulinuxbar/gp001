#include"../pg001/stdafx.h"
#include"myrecord.h"

extern int whichtab;

myrecord::myrecord(CDatabase *db):CRecordset(db)
{
	switch(whichtab)
	{
	case 0://table base_msg
		m_nFields=8;
		memset((void*)&bsd,0,sizeof(bsd));
		break;
	case 1://table data_msg
		m_nFields=3;
		memset((void*)&dv,0,sizeof(dv));
		break;
	};
	m_nDefaultType=snapshot;
};
CString myrecord::GetDefaultSQL()
{
	switch(whichtab)
	{
	case 0:
		return _T("base_msg");
	case 1:
		return _T("data_msg");
	};
	return _T("error");
};
void myrecord::DoFieldExchange(CFieldExchange *pdx)
{
	pdx->SetFieldType(CFieldExchange::outputColumn);
	switch(whichtab)
	{
	case 0:
		RFX_Int(pdx,"ID",bsd.gid);
		RFX_Byte(pdx,"gtype",bsd.gtype);
		RFX_Int(pdx,"gcode",bsd.code);
		RFX_Text(pdx,"gname",bsd.name,60);
		RFX_Byte(pdx,"gwatch",bsd.gwatch);
		RFX_Int(pdx,"gtimes",bsd.gtimes);
		RFX_Double(pdx,"gmax",bsd.gmax);
		RFX_Double(pdx,"gstd",bsd.gstd);
		break;
	case 1:
		RFX_Int(pdx,"ID",dv.gid);
		RFX_Date(pdx,"gdate",dv.tm);
		RFX_Double(pdx,"gvalue",dv.gv);
		break;
	};
};