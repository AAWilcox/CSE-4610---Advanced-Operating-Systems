/*******************************************************************************
* Name:			Alyssa Wilcox
* Id:			006861225
* Date:			March 18, 2021
* Class:		CSE 4610 - Advanced OS
* Assignment:	Project Part 1
* Filename:		Sdisk.cpp
* Purpose:		Contains the member functions of the Sdisk class.
********************************************************************************/

#include "Sdisk.h"

/*******************************************************************************
* Purpose:		The constructor function for the class Sdisk.
* Parameter(s):	diskName: the name of the disk.
*				numberOfBlocks: the number of blocks on the disk.
*				blockSize: the size of the blocks on the disk.
* Return:		None.
********************************************************************************/
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

	//Close the file
	//outfile.close();
}

/*******************************************************************************
* Purpose:		An accessor function for the number of blocks in Sdisk.
* Parameter(s):	None.
* Return:		The number of blocks in Sdisk.
********************************************************************************/
//Accessor
int Sdisk::getNumberOfBlocks()
{
	return numberOfBlocks;
}

/*******************************************************************************
* Purpose:		An accessor function for the block size of blocks in Sdisk.
* Parameter(s):	None.
* Return:		The size of the blocks in Sdisk.
********************************************************************************/
//Accessor
int Sdisk::getBlockSize()
{
	return blockSize;
}

/*******************************************************************************
* Purpose:		Retreives block blockNumber from the disk and stores the data
*				in the string buffer.
* Parameter(s):	blockNumber: the block to read.
*				buffer: a string buffer used to read the block.
* Return:		1 if the block is successfully read.
*				0 if there is an error.
********************************************************************************/
int Sdisk::getBlock(int blockNumber, string& buffer)
{
	//File stream object
	fstream f;

	//Open the file - file must be c string
	f.open(diskName.c_str(), ios::in | ios::out);

	//Position of block
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

	//Close the file
	f.close();

	//Successful, all flags are good
	if (f.good())
		return 1;
	//Error, a flag has been set
	else if (f.bad())
		return 0;
}

/*******************************************************************************
* Purpose:		Writes the string buffer to block blockNumber.
* Parameter(s):	blockNumber: the block to write to.
*				buffer: a string buffer holding the contents to write to a block.
* Return:		1 if the block is successfully written to.
*				0 if there is an error.
********************************************************************************/
int Sdisk::putBlock(int blockNumber, string buffer)
{
	//File stream object
	fstream f;

	//Open the file - file must be c string
	f.open(diskName.c_str(), ios::in | ios::out);

	//Position of block
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