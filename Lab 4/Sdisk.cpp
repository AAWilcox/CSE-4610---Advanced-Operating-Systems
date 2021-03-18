//Alyssa Wilcox
//March 3, 2021
//CSE 4610 - Advanced OS
//Project Part 1
//Sdisk.cpp
//Contains the functionalities of the Sdisk class

#include "Sdisk.h"

//Constructor
Sdisk::Sdisk(string diskName, int numberOfBlocks, int blockSize)
{
	//Set diskname
	this->diskName = diskName;
	//Set number of blocks
	this->numberOfBlocks = numberOfBlocks;
	//Set blocksize
	this->blockSize = blockSize;

	//Build a file with numberOfBocks * blockSize number of # symbols
	ofstream outfile;
	outfile.open(this->diskName);
	//Number of rows
	for (int i = 0; i < numberOfBlocks * blockSize; i++)
	{
		outfile.put('#');
	}
}

//Accessor
int Sdisk::getNumberOfBlocks()
{
	return numberOfBlocks;
}

//Accessor
int Sdisk::getBlockSize()
{
	return blockSize;
}

//Read a block
int Sdisk::getBlock(int blockNumber, string& buffer)
{
	//File stream object
	fstream f;

	//Open the file - file must be c string
	f.open(diskName.c_str(), ios::in | ios::out);

	//Position of block
	//The k blocknumber is found at position k- * blockSize
	int position = blockNumber * blockSize;

	//Find the block to read in
	f.seekg(position);

	//Get blockSize number of characters in block, push to buffer
	for (int i = 0; i < blockSize; i++)
	{
		//Get a character from the block
		char c = f.get();

		//Push character to buffer
		buffer.push_back(c);
	}

	f.close();

	//Successful, all flags are good
	if (f.good())
		return 1;
	//Error, a flag has been set
	else if (f.bad())
		return 0;
}

//Write a block
int Sdisk::putBlock(int blockNumber, string buffer)
{
	//File stream object
	fstream f;

	//Open the file - file must be c string
	f.open(diskName.c_str(), ios::in | ios::out);

	//Position of block
	//The k blocknumber is found at position (k-1) * blockSize
	int position = blockNumber * blockSize;

	//Find the block to be written to
	f.seekg(position);

	//Write to the block
	for (int i = 0; i < blockSize; i++)
	{
		//Write ith character of buffer to file
		f.put(buffer[i]);
	}

	//Close the file
	f.close();

	//Successful, all flags are good
	if (f.good())
		return 1;
	//Error, a flag has been set
	else if (f.bad())
		return 0;
}