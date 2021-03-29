//Alyssa Wilcox
//March 18, 2021
//CSE 4610 - Advanced OS
//Lab 5
//Filesys.h
//Implement a class that will be used to simulate a simple file system/
//In particular, it should handle dynamic file management

//Prevent module conflict
#ifndef FILESYS_H
#define FILESYS_H

#include"Sdisk.h"
#include<vector>

//Filesys class inherits the Sdisk in which the filesys will be placed
class Filesys : public Sdisk
{
public:
	//Constructor - must have all attributes of Sdisk
	Filesys(string diskname, int numberOfBlocks, int blockSize);
	//Write FAT and ROOT to the Sdisk (closing the disk)
	int fsclose();
	//Make a new file
	int newFile(string file);
	//Remove a file
	int rmfile(string file);
	//Get the index a file's first block
	int getFirstBlockNum(string file);
	//Add a block to a file
	int addBlock(string file, string blockNumber);
	//Delete a block from a file
	int delBlock(string file, int blockNumber);
	//Read a block of a file by writing it to buffer
	int readBlock(string file, int blockNumber, string& buffer);
	//Write to a block (whatever is in buffer) of a file
	int writeBlock(string file, int blockNumber, string buffer);
	//Returns the next block that is in the file
	int nextBlock(string file, int blockNumber);
	//For testing purposes
	void printROOT();
	void printFAT();
private:
	//Maximum number of entries in ROOT - root directory
	//The maximum number of files we can have
	int rootSize;
	//Number of blocks occupied by FAT
	//Number of blocks needed to store fat on Sdisk
	//This is not the number of elements in vector fat
	int fatSize;
	//Filenames in ROOT
	vector<string> filename;
	//First blocks in ROOT
	vector<int> firstBlock;
	//FAT - file allocation table
	//Size of fat vector = number of blocks in Sdisk
	vector<int> fat;
	//Build a filesystem
	void buildFS();
	//Read the filesystem
	void readFS();
	//Write filename and firstBlock to Sdisk
	void fssynch();
	//Checks if a block belongs to a file
	bool checkBlock(string file, int blockNumber);
};

#endif
