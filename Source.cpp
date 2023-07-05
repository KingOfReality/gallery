#include "sqlite3.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;
int callback(void* data, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)
	{
		cout << azColName[i] << " = " << argv[i] << " , ";
	}
	cout << endl;
	return 0;
}
/*
int main()
{
	sqlite3* db;
	std::string dbFileName = "galleryDB.sqlite";
	int res = sqlite3_open(dbFileName.c_str(), &db);
	
	
	char* errMessage = nullptr;
	
	std::string state = "  SELECT COUNT(USER_ID) AS MX, USER_ID\nFROM TAGS\nGROUP BY USER_ID\n";
		
	//state += '3';
	//state += ";";
	errMessage = nullptr;
	res = sqlite3_exec(db, state.c_str(), callback, nullptr, &errMessage);
	if (res != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", errMessage);
		sqlite3_free(errMessage);
	}
	sqlite3_close(db);
	db = nullptr;
	return 0;
}*/