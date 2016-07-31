#include "stdafx.h"
#include "SQLiteHelper.h"

#include "FileUtil.h"

#include <iostream>
#include <exception>
#include <memory>

void FunctionWithLocalVariable(Kompex::SQLiteStatement *stmt);

static const TCHAR DB_NAME[] = L"test.db";

class kiteSQLiteHelperFixture : public testing::Test
{
public:
	kiteSQLiteHelperFixture() {}
	virtual ~kiteSQLiteHelperFixture() {}

	virtual void SetUp()
	{
		m_cerrRedirection.reset(new Kompex::CerrRedirection("error.log"));
		m_coutRedirection.reset(new Kompex::CoutRedirection("output.log"));

		// create and open database
		m_DB.reset(new light::db::SQLite::Helper);
	}

	virtual void TearDown()
	{
		m_DB.reset();
		light::FileUtil::remove(DB_NAME);
	}

protected:
	std::auto_ptr<light::db::SQLite::Helper> m_DB;

	std::auto_ptr<Kompex::CoutRedirection> m_coutRedirection;

	std::auto_ptr<Kompex::CerrRedirection> m_cerrRedirection;
};

TEST_F(kiteSQLiteHelperFixture, Dump)
{
	ASSERT_TRUE(m_DB->Init(DB_NAME));
	ASSERT_TRUE(m_DB->Save());
}

TEST_F(kiteSQLiteHelperFixture, TableCreate)
{
	ASSERT_TRUE(m_DB->Init(DB_NAME));
	ASSERT_TRUE(m_DB->Execute(L"CREATE TABLE user (userID INTEGER NOT NULL PRIMARY KEY, lastName VARCHAR(50) NOT NULL, firstName VARCHAR(50), age INTEGER, weight DOUBLE)"));
	ASSERT_TRUE(m_DB->Save());
}

TEST_F(kiteSQLiteHelperFixture, TableCreateAndInsert)
{
	ASSERT_TRUE(m_DB->Init(DB_NAME));

	ASSERT_TRUE(m_DB->Execute(L"CREATE TABLE user (userID INTEGER NOT NULL PRIMARY KEY, lastName VARCHAR(50) NOT NULL, firstName VARCHAR(50), age INTEGER, weight DOUBLE)"));
	ASSERT_TRUE(m_DB->Execute(L"INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (1, 'Lehmann', 'Jamie', 20, 65.5)"));
	ASSERT_TRUE(m_DB->Execute(L"INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (2, 'Burgdorf', 'Peter', 55, NULL)"));

	ASSERT_TRUE(m_DB->Save());
}

TEST_F(kiteSQLiteHelperFixture, FetchRow)
{
	light::db::SQLite::Helper helper;
	ASSERT_TRUE(helper.Init(L"test.db"));

	ASSERT_TRUE(helper.Execute(L"CREATE TABLE user (userID INTEGER NOT NULL PRIMARY KEY, lastName VARCHAR(50) NOT NULL, firstName VARCHAR(50), age INTEGER, weight DOUBLE)") );
	ASSERT_TRUE(helper.Execute(L"INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (1, 'Lehmann', 'Jamie', 20, 65.5)") );
	ASSERT_TRUE(helper.Execute(L"INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (2, 'Burgdorf', 'Peter', 55, NULL)") );
	ASSERT_TRUE(helper.Execute(L"INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (3, 'Lehmann', 'Fernando', 18, 70.2)") );
	ASSERT_TRUE(helper.Execute(L"INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (4, 'Lehmann', 'Carlene ', 17, 50.8)") );

	std::wstring userID;
	std::wstring lastName;
	std::wstring firstName;
	INT age;
	INT64 weight;

	ASSERT_TRUE(helper.Select(L"SELECT * FROM user"));
	helper.Bind(L"userID", userID);
	helper.Bind(L"lastName", lastName);
	helper.Bind(L"firstName", firstName);
	helper.Bind(L"age", age);
	helper.Bind(L"weight", weight);

	INT RowCount = 0;
	while(helper.Next())
	{
		wprintf(L"userID:%s, lastName:%s, firstName:%s, age:%d, weight:%I64d\n", userID.c_str(), lastName.c_str(), firstName.c_str(), age, weight);
		++RowCount;
	}

	ASSERT_EQ(RowCount, 4); // 4번 INSERT 했었으므로.
}