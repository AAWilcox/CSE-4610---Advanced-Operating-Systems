/*******************************************************************************
* Name:			Alyssa Wilcox
* Id:			006861225
* Date:			May 10, 2021
* Class:		CSE 4610 - Advanced OS
* Assignment:	Lab 8
* Filename:		Table.h
* Purpose:		Implement the table class. All functions return 1 if successful
*				and 0 otherwise. Work on the IndexSearch and Search functions.
********************************************************************************/

//Prevent module conflict
#ifndef TABLE_H
#define TABLE_H

//#include"Filesys.h"
#include"Shell.h"

//Table class, inherits Shell
class Table : public Shell
{
public:
	//Constructor
	Table(string diskName, int numberOfBlocks, int blockSize, string flatFile, string indexFile);
	//Reads records from input file, add the records to the flatfile, create index records
	//consisting of the date and block number, and then add the index records
	//to the index file
	int Build_Table(string input_file);
	//Accepts a key value, and searches the index file
	int Search(string key);

private:
	//The name of our flat file on our file system
	string flatFile;
	//The name of our index file on our file system
	string indexFile;
	//The number of records
	int numberOfRecords;
	//Accepts a key value and searches the index file for that key
	//Reads in the index file, goes through all the keys to look for the
	//passed in key, and it will return the block number that has that record
	int IndexSearch(string value);
};

#endif
