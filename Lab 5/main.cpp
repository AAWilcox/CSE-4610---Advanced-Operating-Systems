//Alyssa Wilcox
//March 18, 2021
//CSE 4610 - Advanced OS
//Lab 5
//main.cpp
//In Filesys.cpp, work on checkBlock, readBlock, writeBlock, and nextBlock
//Use the main.cpp provided to test your Filesys class

#include"Sdisk.h"
#include"Filesys.h"
#include"Block.h"

int main()
{
	//Create the disk
	//256 blocks, block size of 128 characters (bytes)
	Sdisk disk1("disk1", 256, 128);
	//Create the filesystem
	Filesys fsys("disk1", 256, 128);
	//Block object
	Block blk;

	//fsys.printROOT();
	//fsys.printFAT();

	string buffer;

	fsys.getBlock(1, buffer);
	cout << "Block 1 in main: " << endl;
	cout << buffer << endl;

	//Create a new file named file1
	fsys.newFile("file1");
	//Create a new file named file2
	fsys.newFile("file2");

	//Buffer for file1
	string bfile1;
	//Buffer for file2
	string bfile2;

	//Add 1024 "1"s to buffer for file1
	for (int i = 1; i <= 1024; i++)
	{
		bfile1 += "1";
	}

	//Break buffer into blocks
	vector<string> blocks = blk.block(bfile1, 128);

	int blocknumber = 0;

	//Add blocks to file1
	for (int i = 0; i < blocks.size(); i++)
	{
		//addBlock returns block number that was allocated
		blocknumber = fsys.addBlock("file1", blocks[i]);
	}

	//Delete the first block of file1
	fsys.delBlock("file1", fsys.getFirstBlockNum("file1"));

	//Add 2024 "2"s to buffer for file2
	for (int i = 1; i <= 2048; i++)
	{
		bfile2 += "2";
	}

	//Break buffer into blocks
	blocks = blk.block(bfile2, 128);

	//Add blocks to file2
	for (int i = 0; i < blocks.size(); i++)
	{
		//addBlock returns block number that was allocated
		blocknumber = fsys.addBlock("file2", blocks[i]);
	}

	//Delete the last block of file2
	fsys.delBlock("file2", blocknumber);

	//Read block 1 of file1 - should return an error
	//Block 1 does not belong to file1
	string buffer3;
	fsys.readBlock("file1", 1, buffer3);
	buffer3.clear();
	//Read block 14 of file1
	fsys.readBlock("file1", 14, buffer3);
	cout << "Block 14 of file1 contains:" << endl;
	cout << buffer3 << endl;

	//Write to a block
	string buffer4;
	for (int i = 0; i < 128; i++)
		buffer4 += "3";
	//Write to block 2 of file1 - should return an error
	fsys.writeBlock("file1", 2, buffer4);
	fsys.writeBlock("file1", 14, buffer4);
	buffer4.clear();
	//Write to block 14 of file1
	fsys.readBlock("file1", 14, buffer4);
	cout << "Block 14 of file1 contains:" << endl;
	cout << buffer4 << endl;

	//Find the next block of block 21 in file2
	int next = fsys.nextBlock("file2", 21);
	cout << "Next block of 21 is: " << next << endl;
	//Find the next block of block 20 in file2 - should return an error
	next = fsys.nextBlock("file2", 20);
	cout << "Next block of 20 is: " << next << endl;
	//Find the next block of block 34 in file2 - this is the last block
	next = fsys.nextBlock("file2", 34);
	cout << "Next block of 34 is: " << next << endl;

	//Remove a file - should return an error
	fsys.rmfile("file1");
	fsys.newFile("file3");
	cout << "Attempting to remove file3" << endl;
	fsys.rmfile("file3");

	
	fsys.printROOT();
	fsys.printFAT();

	string buffer10;

	for (int i = 12; i <= 36; i++)
	{
		fsys.getBlock(i, buffer10);
		cout << "Block " << i << ": " << buffer10 << endl;
		buffer10.clear();
	}

	return 0;
}


