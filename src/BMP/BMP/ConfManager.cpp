#include "StdAfx.h"
#include "ConfManager.h"

//数据库文件名
#define DB_NAME		_T("bmp.db")

//配置表名称
#define CONFIGURE_TABLE_NAME		_T("configure")

//配置表各列名称
#define CONFIGURE_COLUMN_NAME		_T("name")
#define CONFIGURE_COLUMN_VALUE			_T("value")

//配置项名称
#define DATABASE_SCHEME_VERSION	_T("DBSchemeVersion")  //数据库模式版本号，以便后续数据库兼容问题考虑

#define GOLDCODE						_T("GoldCode")  //五行
#define WOODCODE					_T("WoodCode")
#define WATERCODE					_T("WaterCode")
#define FIRECODE						_T("FireCode")
#define EARTHCODE					_T("EarthCode")

#define IS_SOUND_CHECK		_T("IsSoundCheck")

#define AVG_DISCOUNT			_T("AvgDiscount")		//平均回水
#define AVG_BETTING_ODDS	_T("AvgBettingOdds")	//平均赔率

#define WIN_CODE			_T("WinCode")			//中奖号码

#define SELFBUY_DISCOUNT		_T("SelfBuyDiscount")		//自己购买回水和赔率
#define SELFBUY_BETTINGODDS		_T("SelfBuyBettingOdds")

#define NEED_HINT_CONFIGURE_WU_HANG		_T("IsNeedHintConfigureWuHang")  //是否需要提示配置五行

//五行默认号码
#define GOLD_DEFAULT_CODE		_T("13 14 21 22 29 30 43 44")
#define WOOD_DEFAULT_CODE		_T("03 04 11 12 25 26 33 34 41 42")
#define WATER_DEFAULT_CODE		_T("01 02 09 10 17 18 31 32 39 40 47 48")
#define FIRE_DEFAULT_CODE			_T("05 06 19 20 27 28 35 36 49")
#define EARTH_DEFAULT_CODE		_T("07 08 15 16 23 24 37 38 45 46")

CCS CConfManager::m_synCCS;
CConfManager::CConfManager(void)
{
}

CConfManager::~CConfManager(void)
{
}

CConfManager* CConfManager::Instance()
{
	CSynObject synObj(&m_synCCS);

	static CConfManager thisObj;
	return &thisObj;
}

void CConfManager::Init()
{
	CSynObject synObj(&m_synCCS);

	//创建配置表并读取配置数据
	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return;
	}

	vector<COLUMN_ITEM> columnVec;
	COLUMN_ITEM nameColumn;
	nameColumn.strName = CONFIGURE_COLUMN_NAME;
	nameColumn.dataType = DB_DATA_TYPE_STR;
	nameColumn.IsPrimaryKey = true;
	columnVec.push_back(nameColumn);

	COLUMN_ITEM valueColumn;
	valueColumn.strName = CONFIGURE_COLUMN_VALUE;
	valueColumn.dataType = DB_DATA_TYPE_STR;	
	columnVec.push_back(valueColumn);	

	nResult = dbUtil.CreateTable(CONFIGURE_TABLE_NAME, columnVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to create configure table, error=%d."), nResult);
		return;
	}
	
	m_confMap.clear();
	vector<COLUMN_ITEM> whereVec;
	nResult = dbUtil.Query(CONFIGURE_TABLE_NAME, whereVec, EnumRecord, (LPARAM)this);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to query configure, error=%d."), nResult);
		return;
	}
}

bool CConfManager::EnumRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec)
{
	CConfManager* thisObj = (CConfManager*)context;
	CString strName, strValue;
	for (vector<COLUMN_ITEM>::const_iterator it=recordVec.begin(); it!=recordVec.end(); it++)
	{
		if (it->strName == CONFIGURE_COLUMN_NAME)
		{
			strName = it->strValue;
		}
		else if (it->strName == CONFIGURE_COLUMN_VALUE)
		{
			strValue = it->strValue;
		}		
	}

	thisObj->m_confMap.insert(map<CString, CString>::value_type(strName, strValue));

	return true;
}

int CConfManager::DBSchemeVersion() const
{
	CSynObject synObj(&m_synCCS);

	map<CString, CString>::const_iterator it = m_confMap.find(DATABASE_SCHEME_VERSION);
	if (it == m_confMap.end())  //没有配置，使用默认值
	{
		return 1;  //默认为1
	}
	else
	{
		return _ttoi(it->second);
	}
}

bool CConfManager::DBSchemeVersion(int nVersion)
{
	CSynObject synObj(&m_synCCS);

	return Save(DATABASE_SCHEME_VERSION, CStringUtil::IntToStr(nVersion));
}

bool CConfManager::IsWuHangDefaultValue() const
{
	CSynObject synObj(&m_synCCS);

	if (GoldCode() == GOLD_DEFAULT_CODE && WoodCode() == WOOD_DEFAULT_CODE && WaterCode() == WATER_DEFAULT_CODE && FireCode() == FIRE_DEFAULT_CODE && EarthCode() == EARTH_DEFAULT_CODE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

CString CConfManager::GoldCode() const
{
	CSynObject synObj(&m_synCCS);

	map<CString, CString>::const_iterator it = m_confMap.find(GOLDCODE);
	if (it == m_confMap.end())  //没有配置，使用默认值
	{
		return GOLD_DEFAULT_CODE;
	}
	else
	{
		return it->second;
	}
}

bool CConfManager::GoldCode(const CString& strCode)
{
	CSynObject synObj(&m_synCCS);

	return Save(GOLDCODE, strCode);
}


CString CConfManager::WoodCode() const
{
	CSynObject synObj(&m_synCCS);

	map<CString, CString>::const_iterator it = m_confMap.find(WOODCODE);
	if (it == m_confMap.end())
	{
		return WOOD_DEFAULT_CODE;
	}
	else
	{
		return it->second;
	}
}

bool CConfManager::WoodCode(const CString& strCode)
{
	CSynObject synObj(&m_synCCS);

	return Save(WOODCODE, strCode);
}

CString CConfManager::WaterCode() const
{
	CSynObject synObj(&m_synCCS);

	map<CString, CString>::const_iterator it = m_confMap.find(WATERCODE);
	if (it == m_confMap.end())
	{
		return WATER_DEFAULT_CODE;
	}
	else
	{
		return it->second;
	}
}

bool CConfManager::WaterCode(const CString& strCode)
{
	CSynObject synObj(&m_synCCS);

	return Save(WATERCODE, strCode);
}

CString CConfManager::FireCode() const
{
	CSynObject synObj(&m_synCCS);

	map<CString, CString>::const_iterator it = m_confMap.find(FIRECODE);
	if (it == m_confMap.end())
	{
		return FIRE_DEFAULT_CODE;
	}
	else
	{
		return it->second;
	}
}

bool CConfManager::FireCode(const CString& strCode)
{
	CSynObject synObj(&m_synCCS);

	return Save(FIRECODE, strCode);
}

CString CConfManager::EarthCode() const
{
	CSynObject synObj(&m_synCCS);

	map<CString, CString>::const_iterator it = m_confMap.find(EARTHCODE);
	if (it == m_confMap.end())
	{
		return EARTH_DEFAULT_CODE;
	}
	else
	{
		return it->second;
	}
}

bool CConfManager::EarthCode(const CString& strCode)
{
	CSynObject synObj(&m_synCCS);

	return Save(EARTHCODE, strCode);
}

bool CConfManager::IsSoundCheck() const
{
	CSynObject synObj(&m_synCCS);

	map<CString, CString>::const_iterator it = m_confMap.find(IS_SOUND_CHECK);
	if (it == m_confMap.end())
	{
		return true;  //默认是开启的
	}
	else
	{
		return it->second == _T("1") ? true:false;
	}
}

bool CConfManager::SoundCheck(bool bEnable)
{
	CSynObject synObj(&m_synCCS);

	return Save(IS_SOUND_CHECK, bEnable? _T("1"):_T("0"));
}

int CConfManager::AvgDiscount() const
{
	CSynObject synObj(&m_synCCS);

	map<CString, CString>::const_iterator it = m_confMap.find(AVG_DISCOUNT);
	if (it == m_confMap.end())
	{
		return 100;  //默认是100
	}
	else
	{
		return _ttoi(it->second);
	}
}

bool CConfManager::AvgDiscount(int value)
{
	CSynObject synObj(&m_synCCS);

	return Save(AVG_DISCOUNT, CStringUtil::IntToStr(value));
}

int CConfManager::AvgBettingOdds() const
{
	CSynObject synObj(&m_synCCS);

	map<CString, CString>::const_iterator it = m_confMap.find(AVG_BETTING_ODDS);
	if (it == m_confMap.end())
	{
		return 41;  //默认是41
	}
	else
	{
		return _ttoi(it->second);
	}
}

bool CConfManager::AvgBettingOdds(int value)
{
	CSynObject synObj(&m_synCCS);

	return Save(AVG_BETTING_ODDS, CStringUtil::IntToStr(value));
}

int CConfManager::WinCode() const
{
	CSynObject synObj(&m_synCCS);

	map<CString, CString>::const_iterator it = m_confMap.find(WIN_CODE);
	if (it == m_confMap.end())
	{
		return 0;  //默认是0
	}
	else
	{
		return _ttoi(it->second);
	}
}

bool CConfManager::WinCode(int value)
{
	CSynObject synObj(&m_synCCS);

	return Save(WIN_CODE, CStringUtil::IntToStr(value));
}

int CConfManager::SelfBuyDiscount() const
{
	CSynObject synObj(&m_synCCS);

	map<CString, CString>::const_iterator it = m_confMap.find(SELFBUY_DISCOUNT);
	if (it == m_confMap.end())
	{
		return 100;  //默认是100
	}
	else
	{
		return _ttoi(it->second);
	}
}

bool CConfManager::SelfBuyDiscount(int value)
{
	CSynObject synObj(&m_synCCS);

	return Save(SELFBUY_DISCOUNT, CStringUtil::IntToStr(value));
}

int CConfManager::SelfBuyBettingOdds() const
{
	CSynObject synObj(&m_synCCS);

	map<CString, CString>::const_iterator it = m_confMap.find(SELFBUY_BETTINGODDS);
	if (it == m_confMap.end())
	{
		return 41;  //默认是41
	}
	else
	{
		return _ttoi(it->second);
	}
}

bool CConfManager::SelfBuyBettingOdds(int value)
{
	CSynObject synObj(&m_synCCS);

	return Save(SELFBUY_BETTINGODDS, CStringUtil::IntToStr(value));
}

bool CConfManager::IsNeedHintConfigureWuHang() const
{
	CSynObject synObj(&m_synCCS);

	map<CString, CString>::const_iterator it = m_confMap.find(NEED_HINT_CONFIGURE_WU_HANG);
	if (it == m_confMap.end())
	{
		return true;  //默认需要
	}
	else
	{
		return _ttoi(it->second)? true:false;
	}
}

bool CConfManager::NeedHintConfigureWuHang(bool bNeed)
{
	CSynObject synObj(&m_synCCS);

	return Save(NEED_HINT_CONFIGURE_WU_HANG, bNeed? _T("1"):_T("0"));
}

bool CConfManager::Save(const CString& strName, const CString& strValue)
{
	//保存到数据库中
	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return false;
	}

	vector<COLUMN_ITEM> columnVec;
	COLUMN_ITEM nameColumn;
	nameColumn.strName = CONFIGURE_COLUMN_NAME;
	nameColumn.strValue = strName;
	columnVec.push_back(nameColumn);

	COLUMN_ITEM valueColumn;
	valueColumn.strName = CONFIGURE_COLUMN_VALUE;
	valueColumn.strValue = strValue;
	columnVec.push_back(valueColumn);	

	nResult = dbUtil.Insert(CONFIGURE_TABLE_NAME, columnVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to insert configrue(%s) to database, error=%d."), strName, nResult);
		return false;
	}

	//如果该配置不存在加入，存在修改它
	map<CString, CString>::iterator it = m_confMap.find(strName);
	if (it == m_confMap.end())
	{
		m_confMap.insert(map<CString, CString>::value_type(strName, strValue));
	}
	else
	{
		it->second = strValue;
	}

	return true;
}