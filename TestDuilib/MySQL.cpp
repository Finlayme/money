#include "MySQL.h"
#include <iostream>
using namespace std;


MySQL::MySQL()
{
	_mySQL = mysql_init(nullptr);
}

bool MySQL:: ConnectMySQL(const char* host, const char* user, const char* passward, const char* dbName)
{
	if (!mysql_real_connect(_mySQL, host, user, passward, dbName, 3306, nullptr, 0))
	{
		cout << "数据库连接失败" << endl;
		return false;
	}

	mysql_query(_mySQL, "set names 'gbk'");
	return true;
}

vector<vector<string>> MySQL::Select(const string& strSQL)
{
	vector<vector<string>> vvRet;
	if (mysql_query(_mySQL, strSQL.c_str()))
	{
		// SQL命令响应失败
		cout << mysql_error(_mySQL) << endl;
		return vvRet;
	}

	// 获取查询的记录集
	MYSQL_RES * mySQLRes = mysql_store_result(_mySQL);
	if (nullptr == mySQLRes)
	{
		cout << mysql_error(_mySQL) << endl;
		return vvRet;
	}

	// 获取记录中有多少个字段
	int itemCount = mysql_num_fields(mySQLRes);
	MYSQL_ROW mysqlRow;

	while (mysqlRow = mysql_fetch_row(mySQLRes))
	{
		// 已经获取到一条记录
		vector<string> vItem;
		for (size_t i = 0; i < itemCount; ++i)
		{
			vItem.push_back(mysqlRow[i]);
		}

		vvRet.push_back(vItem);
	}

	mysql_free_result(mySQLRes);

	return vvRet;
}

bool MySQL::Insert(const string& strSQL)
{
	if (mysql_query(_mySQL, strSQL.c_str()))
	{
		// SQL命令响应失败
		cout << mysql_error(_mySQL) << endl;
		return false;
	}

	return true;
}

bool MySQL::UpDate(const string& strSQL)
{
	if (mysql_query(_mySQL, strSQL.c_str()))
	{
		// SQL命令响应失败
		cout << mysql_error(_mySQL) << endl;
		return false;
	}

	return true;
}

MySQL::~MySQL()
{
	mysql_close(_mySQL);
}