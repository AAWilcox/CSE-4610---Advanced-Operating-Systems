//Alyssa Wilcox
//March 3, 2021
//CSE 4610 - Advanced OS
//Lab 2
//Sdisk.h
//Implement a class that will be used to simulate a disk drive.
//The disk drive will have numberOfBlocks many blocks where
//each block has blockSize bytes.

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
	//Accessor
	int getNumberOfBlocks();
	//Accesor
	int getBlockSize();
	//Read a block
	int getBlock(int blockNumber, string& buffer);
	//Write a block
	int putBlock(int blockNumber, string buffer);
	
private:
	//Name of file that contains our Sdisk
	string diskName;
	int numberOfBlocks;
	int blockSize;
};

#endif
