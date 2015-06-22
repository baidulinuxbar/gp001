#include"../pg001/stdafx.h"
#include"myrecord.h"

extern int whichtab;

myrecord::myrecord(CDatabase *db):CRecordset(db)
{
	switch(whichtab)
	{
	case 0://table base_msg
		m_nFields=10;
		memset((void*)&bsd,0,sizeof(bsd));
		break;
	case 1://table data_msg
		m_nFields=3;
		memset((void*)&dv,0,sizeof(dv));
		break;
	case 2://table code_tab
		m_nFields=4;
		memset((void*)&ct,0,sizeof(ct));
		break;
	case 3://table data_tab
		m_nFields=11;
		memset((void*)&dt,0,sizeof(dt));
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
	case 2:
		return _T("code_tab");
	case 3:
		return _T("data_tab");
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
		RFX_Byte(pdx,"gcycle",bsd.gcycle);
		RFX_Byte(pdx,"gdecay",bsd.gdecay);
		break;
	case 1:
		RFX_Int(pdx,"ID",dv.gid);
		RFX_Date(pdx,"gdate",dv.tm);
		RFX_Double(pdx,"gvalue",dv.gv);
		break;
	case 2://code_tab
		RFX_Date(pdx,"gp_date",ct.gp_date);
		RFX_Int(pdx,"gp_count",ct.gp_count);
		RFX_Date(pdx,"gp_edate",ct.gp_edate);
		RFX_Int(pdx,"gp_ecount",ct.gp_ecount);
		break;
	case 3://data_tab
		RFX_Int(pdx,"gp_code",dt.gp_code);
		RFX_Date(pdx,"gp_date",dt.gp_date);
		RFX_Double(pdx,"gp_pre_price",dt.pre);
		RFX_Double(pdx,"gp_beg_price",dt.beg);
		RFX_Double(pdx,"gp_cur_price",dt.cur);
		RFX_Double(pdx,"gp_cur_max",dt.cur_max);
		RFX_Double(pdx,"gp_cur_min",dt.cur_min);
		RFX_Text(pdx,"gp_total_cnt",dt.total_cnt,20);
		RFX_Text(pdx,"gp_total_price",dt.total_pri,20);
		RFX_Text(pdx,"gp_name",dt.gp_name,60);
		RFX_Byte(pdx,"gp_type",dt.gp_type);
		break;
	};
};