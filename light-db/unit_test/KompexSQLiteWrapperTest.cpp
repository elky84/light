#include "stdafx.h"

#include <iostream>
#include <exception>
#include <memory>

#include "FileUtil.h"


void FunctionWithLocalVariable(Kompex::SQLiteStatement *stmt);

static const WCHAR DB_NAME[] = L"test.db";

class KompexSQLiteWrapperFixture : public testing::Test
{
public:
	KompexSQLiteWrapperFixture() {}
	virtual ~KompexSQLiteWrapperFixture() {}

	virtual void SetUp()
	{
		m_cerrRedirection.reset(new Kompex::CerrRedirection("error.log"));
		m_coutRedirection.reset(new Kompex::CoutRedirection("output.log"));

		// create and open database
		m_DB.reset(new Kompex::SQLiteDatabase(WCHAR_TO_CHAR(DB_NAME), SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, 0));
		m_DB->MoveDatabaseToMemory();

		m_Stmt.reset(new Kompex::SQLiteStatement(m_DB.get()));
	}

	virtual void TearDown()
	{
		m_DB.reset();
		light::FileUtil::remove(DB_NAME);
	}

protected:
	std::auto_ptr<Kompex::SQLiteDatabase> m_DB;

	std::auto_ptr<Kompex::SQLiteStatement> m_Stmt;

	std::auto_ptr<Kompex::CoutRedirection> m_coutRedirection;

	std::auto_ptr<Kompex::CerrRedirection> m_cerrRedirection;
};


TEST_F(KompexSQLiteWrapperFixture, TestDB)
{
	try
	{
		// ---------------------------------------------------------------------------------------------------------
		// create table and insert some data
		m_Stmt->SqlStatement("CREATE TABLE user (userID INTEGER NOT NULL PRIMARY KEY, lastName VARCHAR(50) NOT NULL, firstName VARCHAR(50), age INTEGER, weight DOUBLE)");
		m_Stmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (1, 'Lehmann', 'Jamie', 20, 65.5)");
		m_Stmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (2, 'Burgdorf', 'Peter', 55, NULL)");
		m_Stmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (3, 'Lehmann', 'Fernando', 18, 70.2)");
		m_Stmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (4, 'Lehmann', 'Carlene', 17, 50.8)");

		// ---------------------------------------------------------------------------------------------------------
		// insert some data with Bind..() methods
		m_Stmt->Sql("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES(?, ?, ?, ?, ?);");
		m_Stmt->BindInt(1, 5);
		m_Stmt->BindString(2, "Murahama");
		m_Stmt->BindString(3, "Yura");
		m_Stmt->BindInt(4, 28);
		m_Stmt->BindDouble(5, 60.2);
		// executes the INSERT statement and cleans-up automatically
		m_Stmt->ExecuteAndFree();

		// ---------------------------------------------------------------------------------------------------------
		// lets have a look on a query which is shown in console
		m_Stmt->GetTable("SELECT firstName, userID, age, weight FROM user WHERE lastName = 'Lehmann';", 13);

		// some example SQLite aggregate functions
		ASSERT_EQ(3, m_Stmt->SqlAggregateFuncResult("SELECT COUNT(*) FROM user WHERE lastName = 'Lehmann';"));
		ASSERT_EQ(4, m_Stmt->SqlAggregateFuncResult("SELECT COUNT(weight) FROM user;"));
		ASSERT_EQ(55, m_Stmt->SqlAggregateFuncResult("SELECT MAX(age) FROM user;"));
		ASSERT_EQ(17, m_Stmt->SqlAggregateFuncResult("SELECT MIN(age) FROM user;"));
		ASSERT_EQ(27.6f, m_Stmt->SqlAggregateFuncResult("SELECT AVG(age) FROM user;"));
		ASSERT_EQ(138, m_Stmt->SqlAggregateFuncResult("SELECT SUM(age) FROM user;"));
		ASSERT_EQ(138, m_Stmt->SqlAggregateFuncResult("SELECT TOTAL(age) FROM user;"));

		// ---------------------------------------------------------------------------------------------------------

		// sql query - searching for all people with lastName "Lehmann"
		m_Stmt->Sql("SELECT firstName FROM user WHERE lastName = 'Lehmann';");

		// after a Sql() call we can get some special information
		ASSERT_EQ( std::string("firstName"), m_Stmt->GetColumnName(0));
		ASSERT_EQ( 1, m_Stmt->GetColumnCount());
		ASSERT_EQ( std::string("main"), m_Stmt->GetColumnDatabaseName(0));
		ASSERT_EQ( std::string("user"), m_Stmt->GetColumnTableName(0));
		ASSERT_EQ( std::string("firstName"), m_Stmt->GetColumnOriginName(0));

		// do not forget to clean-up
		m_Stmt->FreeQuery();

		// ---------------------------------------------------------------------------------------------------------
		// another sql query
		m_Stmt->Sql("SELECT * FROM user WHERE firstName = 'Jamie';");

		// after a Sql() call we can get some other special information
		ASSERT_EQ( std::string("userID"), m_Stmt->GetColumnName(0));
		ASSERT_EQ( std::string("lastName"), m_Stmt->GetColumnName(1));
		ASSERT_EQ( std::string("firstName"), m_Stmt->GetColumnName(2));
		ASSERT_EQ( std::string("age"), m_Stmt->GetColumnName(3));
		ASSERT_EQ( std::string("weight"), m_Stmt->GetColumnName(4));
		ASSERT_EQ( 5, m_Stmt->GetColumnCount());
		ASSERT_EQ( std::string("INTEGER"), m_Stmt->GetColumnDeclaredDatatype(0));
		ASSERT_EQ( std::string("VARCHAR(50)"), m_Stmt->GetColumnDeclaredDatatype(1));
		ASSERT_EQ( std::string("VARCHAR(50)"), m_Stmt->GetColumnDeclaredDatatype(2));
		ASSERT_EQ( std::string("INTEGER"), m_Stmt->GetColumnDeclaredDatatype(3));
		ASSERT_EQ( std::string("DOUBLE"), m_Stmt->GetColumnDeclaredDatatype(4));

		// process all results
		while(m_Stmt->FetchRow())
		{
			ASSERT_EQ(5, m_Stmt->GetDataCount());
			ASSERT_EQ(1, m_Stmt->GetColumnDouble(0));
			ASSERT_EQ(std::string("Lehmann"), m_Stmt->GetColumnString(1));
			ASSERT_EQ(std::string("Jamie"), m_Stmt->GetColumnString(2));
			ASSERT_EQ(std::string("20"), m_Stmt->GetColumnString(3));
			ASSERT_EQ(std::string("65.5"), m_Stmt->GetColumnString(4));

			ASSERT_EQ(1, m_Stmt->GetColumnType(0));
			ASSERT_EQ(3, m_Stmt->GetColumnType(1));
			ASSERT_EQ(3, m_Stmt->GetColumnType(2));
			ASSERT_EQ(1, m_Stmt->GetColumnType(3));
			ASSERT_EQ(2, m_Stmt->GetColumnType(4));
		}

		// do not forget to clean-up
		m_Stmt->FreeQuery();

		// ---------------------------------------------------------------------------------------------------------
		// a little example how to get some queried data via column name
		m_Stmt->Sql("SELECT * FROM user WHERE lastName = 'Lehmann';");

		// process all results

		static const char* LehmannFirstName[] = {"Jamie", "Fernando", "Carlene"};
		static const int LehmanAge[] = {20, 18, 17};

		int n = 0;
		while(m_Stmt->FetchRow())
		{
			ASSERT_EQ( std::string(LehmannFirstName[n]), m_Stmt->GetColumnString("firstName"));
			ASSERT_EQ(LehmanAge[n], m_Stmt->GetColumnInt("age"));
			++n;
		}

		// do not forget to clean-up
		m_Stmt->FreeQuery();

		// ---------------------------------------------------------------------------------------------------------
		// example for prepared statements - repetitive execution (SELECT)
		m_Stmt->Sql("SELECT * FROM user WHERE userID=@id");

		static const char* strIndex[] = {"1", "2", "3"};
		static const char* strUserID[] = {"Lehmann", "Burgdorf", "Lehmann"};
		for(int i = 1; i <= 3; ++i)
		{
			// bind an integer to the prepared statement
			m_Stmt->BindInt(1, i);

			// and now fetch all results
			while(m_Stmt->FetchRow())
			{
				ASSERT_EQ( 0, _stricmp(strIndex[i - 1], (const char*)m_Stmt->GetColumnCString(0)) );
				ASSERT_EQ( 0, _stricmp(strUserID[i - 1], (const char*)m_Stmt->GetColumnCString(1)) );
			}

			// reset the prepared statement
			m_Stmt->Reset();
		}
		// do not forget to clean-up
		m_Stmt->FreeQuery();

		// ---------------------------------------------------------------------------------------------------------
		// two possibilities to update data in the database

		// the first way
		m_Stmt->Sql("UPDATE user SET lastName=@lastName, age=@age WHERE userID=@userID");

		// bind an integer to the prepared statement
		m_Stmt->BindString(1, "Urushihara");		// bind lastName
		m_Stmt->BindInt(2, 56);					// bind age
		m_Stmt->BindInt(3, 2);					// bind userID

		// execute it and clean-up
		m_Stmt->ExecuteAndFree();

		// the second way
		m_Stmt->SqlStatement("UPDATE user SET weight=51.5, age=18 WHERE firstName='Carlene'");

		// ---------------------------------------------------------------------------------------------------------
		// get some instant results
		ASSERT_EQ(std::string("Lehmann"), m_Stmt->GetSqlResultString("SELECT lastName FROM user WHERE userID = 3"));
		ASSERT_EQ(18, m_Stmt->GetSqlResultInt("SELECT age FROM user WHERE userID = 4"));
		ASSERT_EQ(70.2, m_Stmt->GetSqlResultDouble("SELECT weight FROM user WHERE userID = 3"));

		// don't forget to delete the pointer for all GetSqlResult%() methods which return a pointer
		const unsigned char *lastName = m_Stmt->GetSqlResultCString("SELECT lastName FROM user WHERE userID = 2");
		// do something with lastName
		delete[] lastName;

		// ---------------------------------------------------------------------------------------------------------
		// DELETE statement and get afterwards the number of affected rows
		m_Stmt->SqlStatement("DELETE FROM user WHERE lastName = 'Lehmann'");
		ASSERT_EQ(3, m_DB->GetDatabaseChanges());

		// let's see, how many changes we have done
		ASSERT_EQ(10, m_DB->GetTotalDatabaseChanges());

		// ---------------------------------------------------------------------------------------------------------
		// get all metadata from one column
		m_Stmt->GetTableColumnMetadata("user", "userID");

		// ---------------------------------------------------------------------------------------------------------
		// now we want try a transaction
		// if an error occurs, a rollback is automatically performed
		// note: you must use Transaction()
		m_Stmt->BeginTransaction();
		m_Stmt->Transaction("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (10, 'Kanzaki', 'Makoto', 23, 76.9)");
		FunctionWithLocalVariable(m_Stmt.get());
		m_Stmt->Transaction("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (12, 'Kanzaki', 'Peter', 63, 101.1)");
		m_Stmt->CommitTransaction();
	}
	catch(Kompex::SQLiteException &exception)
	{
		std::cerr << "\nException Occured" << std::endl;
		exception.Show();
	}
}

TEST_F(KompexSQLiteWrapperFixture, RollbackTest)
{

	// if you want react on errors by yourself, you can use a own try() and catch() block
	// note: you must use SqlStatement()
	try
	{
		m_Stmt->SqlStatement("CREATE TABLE user (userID INTEGER NOT NULL PRIMARY KEY, lastName VARCHAR(50) NOT NULL, firstName VARCHAR(50), age INTEGER, weight DOUBLE)");

		m_Stmt->BeginTransaction();
		m_Stmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (10, 'Kanzaki', 'Makoto', 23, 76.9)");
		m_Stmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (11, 'Kanzaki', 'Yura', 20, 56.9)");
		m_Stmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (12, 'Kanzaki', 'Peter', 63, 101.1)");
		m_Stmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (10, 'Henschel', 'Robert', 10, 34.5)");
		m_Stmt->CommitTransaction();
	}
	catch(Kompex::SQLiteException &exception) 
	{
		std::cerr << "\nException Occured" << std::endl;
		exception.Show();

		m_Stmt->RollbackTransaction();
		std::cerr << "Rollback was executed!" << std::endl;
	}
}

TEST_F(KompexSQLiteWrapperFixture, PreprareStatement)
{
	// ---------------------------------------------------------------------------------------------------------
	// example for prepared statements - repetitive execution (INSERT/UPDATE/DELETE)
	try
	{

		// create a table structure
		m_Stmt->SqlStatement("CREATE TABLE flower (flowerID INTEGER NOT NULL PRIMARY KEY, name VARCHAR(50) NOT NULL, size DOUBLE)");
		// create the prepared statement
		m_Stmt->Sql("INSERT INTO flower (flowerID, name, size) VALUES (@flowerID, @name, @size)");

		// bind all three values
		m_Stmt->BindInt(1, 1);					// flowerID
		m_Stmt->BindString(2, "rose");			// name
		m_Stmt->BindDouble(3, 50.5);				// size
		// execute the statement and reset the bindings
		m_Stmt->Execute();
		m_Stmt->Reset();

		// here we go - the second round
		m_Stmt->BindInt(1, 2);					// flowerID
		m_Stmt->BindString(2, "primrose");		// name
		m_Stmt->BindDouble(3, 6.21);				// size
		// execute the statement and reset the bindings
		m_Stmt->Execute();
		m_Stmt->Reset();

		// here we go - the second round
		m_Stmt->BindInt(1, 3);					// flowerID
		m_Stmt->BindString(2, "rhododendron");	// name
		m_Stmt->BindDouble(3, 109.84);			// size
		// execute the statement
		m_Stmt->Execute();
		// we don't need the prepared statement anymore so we clean-up everything
		m_Stmt->FreeQuery();
	}
	catch(Kompex::SQLiteException &exception)
	{
		std::cerr << "\nException Occured" << std::endl;
		exception.Show();
	}
}

TEST_F(KompexSQLiteWrapperFixture, BlobTest)
{
	try
	{
		// ---------------------------------------------------------------------------------------------------------
		// Kompex::SQLiteBlob example

		// create a little table and fill it with some dummy data 
		// the content of the BLOBs is simple plaintext so that you can see what happens
		m_Stmt->SqlStatement("CREATE TABLE boarduser (userID INTEGER NOT NULL PRIMARY KEY, username VARCHAR(20), picture BLOB)");
		m_Stmt->SqlStatement("INSERT INTO boarduser (userID, username, picture) VALUES (1, 'apori', 'abcdefghijklmnopqrstuvwxyz')");
		m_Stmt->SqlStatement("INSERT INTO boarduser (userID, username, picture) VALUES (2, 'sarina', 'abcdefghijklmnopqrstuvwxyz')");

		// open the existing BLOB for user 'apori'
		Kompex::SQLiteBlob *pKompexBlob = new Kompex::SQLiteBlob(m_DB.get(), "main", "boarduser", "picture", 1);
		// get the size of the current BLOB
		ASSERT_EQ(26, pKompexBlob->GetBlobSize());

		// read the whole BLOB value
		int blobSize = pKompexBlob->GetBlobSize();
		char *readBuffer = new char[blobSize + 1];
		readBuffer[blobSize] = '\0';
		pKompexBlob->ReadBlob(readBuffer, blobSize);
		ASSERT_EQ(std::string("abcdefghijklmnopqrstuvwxyz"), readBuffer);

		// overwrite a part of the BLOB
		char newData[8] = "-HELLO-";
		pKompexBlob->WriteBlob(newData, sizeof(newData) - 1, 10);

		// and read the whole BLOB value again
		pKompexBlob->ReadBlob(readBuffer, blobSize);
		ASSERT_EQ(std::string("abcdefghij-HELLO-rstuvwxyz"), readBuffer);
		delete readBuffer;

		delete pKompexBlob;

		// ---------------------------------------------------------------------------------------------------------
		// create a sql statement with SQLite functions
		const char *param1 = "It's a happy day.";
		const char *param2 = "Muhahaha!";
		std::cout << "\n" << m_Stmt->Mprintf("INSERT INTO table VALUES('%q', '%q')", param1, param2) << std::endl;
	}
	catch(Kompex::SQLiteException &exception)
	{
		std::cerr << "\nException Occured" << std::endl;
		exception.Show();
	}
}

TEST_F(KompexSQLiteWrapperFixture, MemoryTOFile)
{
	try
	{
		// insert some data sets into the memory database
		m_Stmt->SqlStatement("CREATE TABLE score (id INTEGER NOT NULL PRIMARY KEY, lastScore INTEGER, avgScore DOUBLE)");
		m_Stmt->SqlStatement("INSERT INTO score(id, lastScore, avgScore) VALUES(1, 429, 341)");
		m_Stmt->SqlStatement("INSERT INTO score(id, lastScore, avgScore) VALUES(2, 37, 44)");
		m_Stmt->SqlStatement("INSERT INTO score(id, lastScore, avgScore) VALUES(3, 310, 280)");

		// save the memory database to a file
		// if you don't do it, all database changes will be lost after closing the memory database
		//m_DB->SaveDatabaseFromMemoryToFile(DB_NAME);
	}
	catch(Kompex::SQLiteException &exception)
	{
		std::cerr << "\nException Occured" << std::endl;
		exception.Show();
	}
}

void FunctionWithLocalVariable(Kompex::SQLiteStatement *stmt)
{
	const wchar_t *localVariable = L"INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (11, 'Kanzaki', 'Yura', 20, 56.9)";
	// use SecureTransaction() which creates a internal copy of localVariable
	stmt->SecureTransaction(localVariable);
	// localVariable will be deleted on this end of scope now!
}
