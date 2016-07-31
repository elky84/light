#pragma once

#include "Odbc.h"

class DBCommand
	: public light::db::Odbc::Helper
{
public:
	DBCommand(const light::db::Odbc::ACCOUNT_INFO& AccountInfo)
		: light::db::Odbc::Helper(AccountInfo)
	{

	}

	~DBCommand()
	{

	}

	virtual bool on_update()
	{
		return DoExecute();
	}

	virtual bool DoExecute() = 0;
};

class TestDBCommand : public DBCommand
{
public:
	TestDBCommand(const light::db::Odbc::ACCOUNT_INFO& AccountInfo)
		: DBCommand(AccountInfo)
	{

	}

	virtual bool DoExecute();
};
