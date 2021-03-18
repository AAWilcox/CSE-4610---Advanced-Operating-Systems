//Alyssa Wilcox
//March 16, 2021
//CSE 4610 - Advanced OS
//Lab 4
//main.cpp
//In Filesys.cpp, work on newfile, rmfile, getFirstBlock, and addBlock
//Use the main.cpp provided to test your Filesys class

#include"Sdisk.h"
#include"Filesys.h"

//Block function - breaks an ostringstream buffer into blocks
vector<string> blockt(string buffer, int blockSize)
{
	//Number of blocks
	int numberOfBlocks = 0;

	//Vector of blocks
	vector<string> blocks;

	//If buffer length is divisibly by blockSize
	//We can fill n blocks completely
	if (buffer.length() % blockSize == 0)
	{
		numberOfBlocks = buffer.length() / blockSize;
	}
	//Buffer length not divisible by blockSize
	//We cannot fill all blocks completely - last one will not be completely full
	else
	{
		numberOfBlocks = buffer.length() / blockSize + 1;
	}

	string tempblock;

	//Split buffer into blocks
	for (int i = 0; i < numberOfBlocks; i++)
	{
		//i = 0: position = 0, read blockSize characters
		//i = 1: position = blockSize, read blockSize characters
		//Jumping blockSize characters and reading blockSize characters at a time
		tempblock = buffer.substr(blockSize * i, blockSize);

		//Add block substring to blocks vector
		blocks.push_back(tempblock);
	}

	//Index of last block in vector
	int lastBlock = blocks.size() - 1;

	//Fill in extra space of last block
	for (int j = blocks[lastBlock].length(); j < blockSize; j++)
	{
		blocks[lastBlock] += "#";
	}

	return blocks;
}

int main()
{
    //Create the disk
    //256 blocks, block size of 128 characters (bytes)
    //Sdisk disk1("disk1", 256, 128);
	//Create the filesystem
    Filesys fsys("disk1", 256, 128);

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
    vector<string> blocks = blockt(bfile1, 128);

	int blocknumber = 0;

	//Add blocks to file1
    for (int i = 0; i < blocks.size(); i++)
    {
		//addBlock returns block number that was allocated
        blocknumber = fsys.addBlock("file1", blocks[i]);
    }

	//Delete the first block of file1
    fsys.delBlock("file1", fsys.getFirstBlockNum("file1"));

	//Add 1024 "2"s to buffer for file2
    for (int i = 1; i <= 2048; i++)
    {
        bfile2 += "2";
    }

	//Break buffer into blocks
    blocks = blockt(bfile2, 128);

	//Add blocks to file2
    for (int i = 0; i < blocks.size(); i++)
    {
		//addBlock returns block number that was allocated
        blocknumber = fsys.addBlock("file2", blocks[i]);
    }

	//Delete the last block of file2
    fsys.delBlock("file2", blocknumber);

	fsys.printROOT();
	fsys.printFAT();

	string buffer3;

	for (int i = 13; i <= 34; i++)
	{
		fsys.getBlock(i, buffer3);
		cout << "Block " << i << ": " << buffer3 << endl;
		buffer3.clear();
	}

	return 0;
}


