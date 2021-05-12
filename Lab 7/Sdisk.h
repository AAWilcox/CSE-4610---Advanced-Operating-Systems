/*******************************************************************************
* Name:			Alyssa Wilcox
* Id:			006861225
* Date:			March 18, 2021
* Class:		CSE 4610 - Advanced OS
* Assignment:	Project Part 1
* Filename:		Sdisk.h
* Purpose:		Implement a class that will be used to simulate a disk drive.
*				The disk drive will have numberOfBlocks many blocks where
*				each block has blockSize bytes.
********************************************************************************/

//Prevent module conflict
#ifndef SDISK_H
#define SDISK_H

#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class Sdisk
{
public:
	//Constructor
	Sdisk(string diskname, int numberOfBlocks, int blockSize);
	//Accessor - Returns number of blocks in Sdisk
	int getNumberOfBlocks();
	//Accesor - Returns the size of the blocks in Sdisk
	int getBlockSize();
	//Reads a block in Sdisk
	int getBlock(int blockNumber, string& buffer);
	//Writes a block to Sdisk
	int putBlock(int blockNumber, string buffer);

private:
	//Name of file that contains our Sdisk
	string diskName;
	//The number of blocks in our Sdisk
	int numberOfBlocks;
	//The size of a block in Sdisk
	int blockSize;
};

#endif