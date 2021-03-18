//Alyssa Wilcox
//March 13, 2021
//CSE 4610 - Advanced OS
//Lab 3
//Filesys.h
//Create the Filesys constructor where we create the ROOT directory and the FAT
//Create a function fssynch that writes the ROOT and FAT to the disk

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
	int fsclose();
	//Make a new file
	int newFile(string file);
	//Remove a file
	int rmfile(string file);
	//Get the first block of a file
	int getFirstBlock(string file);
	//Add a block to a file
	int addBlock(string file, string block);
	//Delete a block from a file
	int delBlock(string file, int blockNumber);
	//Read a block of a file by writing it to buffer
	int readBlock(string file, int blockNumber, string& buffer);
	//Write to a block (whatever is in buffer) of a file
	int writeBlock(string file, int blockNumber, string buffer);
	int nextBlock(string file, int blockNumber);
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
	void fssynch();
};

#endif
