/*******************************************************************************
* Name:			Alyssa Wilcox
* Id:			006861225
* Date:			May 10, 2021
* Class:		CSE 4610 - Advanced OS
* Assignment:	Lab 7
* Filename:		Table.cpp
* Purpose:		Implement a table class used to create a database system.
********************************************************************************/

#include"Table.h"
#include"Block.h"
#include<sstream>

Block block;

/*******************************************************************************
* Purpose:		The constructor function for the class Table. Calls Shell's
*				constructor explicitly.
* Parameter(s):	diskName: the name of the disk.
*				numberOfBlocks: the number of blocks in the disk.
*				blockSize: the size of the blocks in the disk.
*				flatFile: the name of the flat file
*				indexFile: the name of the index file
* Return:		None.
********************************************************************************/
Table::Table(string diskName, int numberOfBlocks, int blockSize, string flatFile, string indexFile)
	: Shell(diskName, numberOfBlocks, blockSize)
{
	//Set the flat file name
	this->flatFile = flatFile;

	//Set the index file name
	this->indexFile = indexFile;

	//Set number of records to be 0
	numberOfRecords = 0;

	//Create the flat file and index file
	int b = newFile(flatFile);
	int c = newFile(indexFile);
	if (b == 0 || c == 0)
	{
		cout << "Could not create index or flat file" << endl;
	}
}

/*******************************************************************************
* Purpose:		Reads records from the input file (data.txt), adds the records
*				to the flatFile and create index records consisting of the 
*				date and block number, and then add the index records to the
*				index file.
* Parameter(s):	input_file: The file containing the records to be added to the
*				flat file and processed in the index file.
* Return:		1 if successful, 0 otherwise.
********************************************************************************/
int Table::Build_Table(string input_file)
{
	//Open the input file
	ifstream infile;
	infile.open(input_file.c_str());

	//Could not open the file
	if (infile.bad())
	{
		return 0;
	}

	//Reads the first line of the file
	string s; 
	getline(infile, s);

	//Output buffer used to write to index file
	ostringstream indexstream;

	//Read in the input file as long as we are able to read a line
	while (infile.good())
	{
		//=-=-=-=-= Process The Line =-=-=-=-=
		//Write the record to the flat file and add index record

		//Pull off first 5 bytes (our key - the date)
		string key = s.substr(0, 5);

		//=-=-=-=-= Process Flat File =-=-=-=-=

		//Break the line read in (a record) into blocks
		//We should only get one block back - 128 byte block size, our records are at most 120 bytes
		vector<string> blocks = block.block(s, getBlockSize());

		//Add first block into flat file
		//block[0] is the only block we get back from the block function above
		//b is the block number
		int b = addBlock(flatFile, blocks[0]);

		//File does not exist or block sizes do not match
		if (b <= 0)
		{
			return 0;
		}

		//=-=-=-=-= Process Index File =-=-=-=-=

		//Write to index stream (write the key and the block number in the
		//flat file that contains the record with this key)
		indexstream << key << " " << b << " ";

		//Update the number of records
		numberOfRecords++;

		//Read the next line
		getline(infile, s);
	}

	//Continue processing index file
	//Convert indexstream to a string, write it to buffer
	string buffer = indexstream.str();

	//Break the buffer (what we're going to write to the index file) into blocks
	vector<string> blocks = block.block(buffer, getBlockSize());

	//Write our index file blocks to our index file
	for (int i = 0; i < blocks.size(); i++)
	{
		int b = addBlock(indexFile, blocks[i]);

		//File does not exist or block sizes do not match
		if (b == 0)
		{
			return 0;
		}
	}

	//Success
	return 1;
}