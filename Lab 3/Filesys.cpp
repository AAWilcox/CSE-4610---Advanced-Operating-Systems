//Alyssa Wilcox
//March 3, 2021
//CSE 4610 - Advanced OS
//Lab 3
//Filesys.cpp
//Contains the functionalities of the Filesys class

#include"Filesys.h"
#include<sstream>

//Prototype block function
vector<string> block(string, int);

//Constructor
//Calls Sdisk's constructor explicitly
Filesys::Filesys(string diskname, int numberOfBlocks, int blockSize)
	: Sdisk(diskname, numberOfBlocks, blockSize)
{
	//Every file in the root has 10 characters
	//The maximum number of files we can have
	rootSize = blockSize / 10;

	//Every entry in FAT is 5 digits long
	//getNumberOfBlocks = # of elements in FAT
	//getNumberOfBlocks * 5 = # of bytes/characters in FAT
	//Divide (getNumberOfBlocks*5) by getBlockSize - will tell the
	//number of blocks we absolutely need
	//Give one extra block to FAT
	fatSize = ((getNumberOfBlocks() * 5) / getBlockSize()) + 1;

	//Write what's in a block to the buffer so that we can read the block
	string buffer;
	//getBlock returns 1 if success, 0 if failure
	//Block 0 should be a bunch of # signs
	int code = getBlock(0, buffer);
	//Failure to read the block - Cannot read Sdisk
	if (code == 0)
	{
		cout << "Bad device" << endl;
		return;
	}
	//Can read Sdisk, continue from here

	//Get the first block off the Sdisk
	//First block of Sdisk contains the ROOT
	//buffer[0] contains first character of the file system
	buffer.clear();
	code = getBlock(1, buffer);
	//No file system
	if (buffer[0] == '#')
	{
		//Build a file system
		buildFS();
	}
	//There is a file system
	else
	{
		//Read the file system
		readFS();
	}
}

//Build a filesystem
void Filesys::buildFS()
{
	//=-=-=-=-= BUILD ROOT =-=-=-=-=

	//For every file/entry in ROOT
	for (int i = 0; i < rootSize; i++)
	{
		//Write ROOT to memory
		//All files will have name "xxxx"
		filename.push_back("xxxx");
		//All files will have firstBlock 0
		firstBlock.push_back(0);
	}

	//=-=-=-=-= BUILD FAT =-=-=-=-=

	//Write number of first free block in FAT to fat[0]
	//Number of first free block in FAT = fatSize + 2
	//FAT in memory
	fat.push_back(fatSize + 2);

	//Write 1 to root block (block 1) in FAT
	//FAT in memory
	fat.push_back(1);

	//Starting from block 2 to fatSize - 1, output 1s (symbolize reserved blocks)
	for (int i = 0; i < fatSize; i++)
	{
		//FAT in memory
		fat.push_back(1);
	}

	//Starting at fatSize + 2, we now have empty blocks in FAT
	//From fatSize + 2 to last block in FAT (getNumberOfBlocks - 1)
	for (int i = fatSize + 2; i < getNumberOfBlocks(); i++)
	{
		//Create links for the free list
		//FAT in memory
		fat.push_back(i + 1);
	}

	//Last element in FAT should have zero to symbolize end of free list
	fat[fat.size() - 1] = 0;

	//Write ROOT and FAT (which are in memory) to Sdisk
	fssynch();
}

//Write ROOT and FAT to Sdisk
void Filesys::fssynch()
{
	//=-=-=-=-= Write ROOT to Sdisk =-=-=-=-=

	//Output buffer used to write to Sdisk
	ostringstream ostream;

	//Write ROOT to ostream
	for (int i = 0; i < rootSize; i++)
	{
		ostream << filename[i] << " " << firstBlock[i] << " ";
	}

	//Convert ostream to string, store it in a string called buffer
	string buffer = ostream.str();

	//Split our buffer string into blocks so we can write it to Sdisk
	vector<string> blocks = block(buffer, getBlockSize());

	//blocks[0] contains our ROOT - because ROOT is a single block
	//Put the ROOT into the block 1 of Sdisk
	putBlock(1, blocks[0]);

	//=-=-=-=-= Write FAT to Sdisk =-=-=-=-=

	//Output buffer used to write to Sdisk
	ostringstream ostream2;

	//Write FAT to ostream2
	for (int i = 0; i < fat.size(); i++)
	{
		ostream2 << fat[i] << " ";
	}

	//Convert ostream2 to string, store it in a string called buffer2
	string buffer2 = ostream2.str();

	//Split out buffer2 string into blocks so we can write it to Sdisk
	vector<string> blocks2 = block(buffer2, getBlockSize());

	//Write blocks2 (our FAT split into blocks) to Sdisk
	for (int i = 0; i < blocks2.size(); i++)
	{
		//FAT begins in block 2 of Sdisk
		putBlock(2 + i, blocks2[i]);
	}
}

//Block function - breaks an ostringstream buffer into blocks
vector<string> block(string buffer, int blockSize)
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

//Read the filesystem from Sdisk
void Filesys::readFS()
{
	cout << "Reading from filesystem" << endl;
	//=-=-=-=-= READ IN THE ROOT =-=-=-=-=

	//Input buffer used to read from the disk
	istringstream istream;

	//Buffer to hold ROOT
	string buffer;

	//Get the ROOT, which is in block 1 of Sdisk, write to buffer
	getBlock(1, buffer);

	//Load buffer into istream
	istream.str(buffer);

	//Read in filename and firstBlock of each entry in ROOT
	for (int i = 0; i < rootSize; i++)
	{
		//s is the filename
		string s;
		//t is the firstBlock of that file
		int t;

		//Read in the filename
		istream >> s;
		//Read in the firstBlock
		istream >> t;

		//Add filename to vector
		filename.push_back(s);
		//Add firstBlock to vector
		firstBlock.push_back(t);
	}

	cout << "ROOT:" << endl;
	//Testing
	for (int i = 0; i < filename.size(); i++)
	{
		cout << filename[i] << firstBlock[i] << endl;
	}

	//=-=-=-=-= READ IN THE FAT =-=-=-=-=

	//Input buffer used to read from the disk
	istringstream istream2;

	//Buffer to hold FAT
	string buffer2;

	//Read in each block of the FAT from Sdisk
	for (int i = 0; i < fatSize; i++)
	{
		//String to hold a block of the FAT
		string b;

		//FAT starts on Sdisk block 2
		//Write block to string b
		getBlock(i + 2, b);

		//Add block contents onto buffer
		buffer2 += b;
	}

	//Load buffer into istream2
	istream2.str(buffer2);

	//Read in contents of istream2 (contents of FAT), add to fat vector
	for (int i = 0; i < getNumberOfBlocks(); i++)
	{
		int t;

		//Read from istream2
		istream2 >> t;

		//Add to fat vector
		fat.push_back(t);
	}

	cout << "FAT:" << endl;
	//Testing
	for (int i = 0; i < fat.size(); i++)
	{
		cout << fat[i] << endl;
	}
}
