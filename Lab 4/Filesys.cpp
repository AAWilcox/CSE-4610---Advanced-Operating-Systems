//Alyssa Wilcox
//March 3, 2021
//CSE 4610 - Advanced OS
//Lab 4
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

	getBlock(1, buffer);

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
}

//Create a new file in the file system
//Returns an error code: 1 = success, 0 = failure
int Filesys::newFile(string file)
{
	//Go through ROOT directory
	for (int i = 0; i < filename.size(); i++)
	{
		//Check if the file already exists
		if (filename[i] == file)
		{
			cout << "File exists" << endl;
			return 0;
		}
	}

	//File does not exist - go through ROOT directory
	for (int i = 0; i < filename.size(); i++)
	{
		//We have an empty space at index i of filename vector
		if (filename[i] == "xxxx")
		{
			//Store file's name in filename vector
			filename[i] = file;
			//Set its first block
			firstBlock[i] = 0;
			//Synch the filesystem
			fssynch();
			//Success
			return 1;
		}
	}

	//No room in ROOT
	cout << "No room in root directory" << endl;
	return 0;
}

//Returns index of first block of a file
//Returns an error code: 0 = file is empty, 1 = file doesn't exist
int Filesys::getFirstBlockNum(string file)
{
	//Go through ROOT directory
	for (int i = 0; i < filename.size(); i++)
	{
		//Check if file exists
		if (filename[i] == file)
		{
			//Return that file's firstblock index
			return firstBlock[i];
		}
	}
	//File does not exist
	cout << "No such file exists" << endl;
	return -1;
}

//Removes a file if it is empty
//Returns an error code: 1 = success, 0 = failure
int Filesys::rmfile(string file)
{
	//Go through ROOT directory
	for (int i = 0; i < filename.size(); i++)
	{
		//Check if file exists
		if (filename[i] == file)
		{
			//Check if the file is empty
			if (getFirstBlockNum(file) == 0)
			{
				//File is empty, remove it
				filename[i] == "xxxx";
				//Synch filename vector to Sdisk
				fssynch();
				return 1;
			}
			//File is not empty
			else
			{
				cout << "Cannot remove, the file is not empty" << endl;
				return 0;
			}
		}
	}
	//File does not exist
	cout << "No such file exists" << endl;
	return 0;
}

//Adds a block to a file
//Returns allocated block number
//Returns 0 upon failure
int Filesys::addBlock(string file, string block)
{
	//Check to make sure the block we want to add is the correct block size
	if (block.length() == getBlockSize())
	{
		//First block index of the file
		int first = getFirstBlockNum(file);

		//Check if the file exists
		//getFirstBlockNum returns -1 if file does not exist
		if (first == -1)
		{
			cout << "File does not exist" << endl;
			return 0;
		}
		//File exists
		else
		{
			//Get the first empty block in fat
			int allocate = fat[0];

			//Update the free list in the fat
			fat[0] = fat[fat[0]];

			//Allocate the block - this is now the last block of the file
			fat[allocate] = 0;

			//File was empty, we need to update ROOT with file's new first block
			if (first == 0)
			{
				//Go through the ROOT to find the file
				for (int i = 0; i < filename.size(); i++)
				{
					//We found the file
					if (filename[i] == file)
					{
						//Update the file's firstblock to the block that was just allocated
						firstBlock[i] = allocate;
					}
				}
			}
			//File was not empty, we need to update the FAT
			//Go through FAT and find the old last block of the file, update it
			else
			{
				//First block of our file
				int b = first;

				//Search for the file's old last block in FAT
				//Trace the links of all the blocks that belong to a file
				while (fat[b] != 0)
				{
					b = fat[b];
				}
				//fat[b] == 0, we found the file's old last block. Update it
				fat[b] = allocate;
			}

			//Write the new ROOT/FAT to Sdisk
			fssynch();

			//Write new contents into the allocated block
			putBlock(allocate, block);

			//Success - return index of allocated block
			return allocate;
		}
	}
	//Block sizes do not match
	else
	{
		cout << "Bad block. The block size does not match the disk's block size" << endl;
		return 0;
	}
}

//Checks if a block belongs to a file
//Returns true if block does belong, false otherwise
bool Filesys::checkBlock(string file, int blockNumber)
{
	//Get the first block index of the file
	int b = getFirstBlockNum(file);

	//getFirstBlockNum returns -1 if file does not exist
	if (b == -1)
	{
		return false;
	}

	//first block is the blockNumber we are checking
	if (b == blockNumber)
	{
		return true;
	}

	//Go through the links of the file until we find the index of the 
	//last block of the file.
	//The last block of a file contains 0
	while (fat[b] != 0)
	{
		//blockNumber we are checking belongs to the file
		if (fat[b] == blockNumber)
		{
			return true;
		}

		//Iterate through links
		b = fat[b];
	}

	//Block does not belong to the file
	return false;
}

//Deletes a block from a file
int Filesys::delBlock(string file, int blockNumber)
{
	//Check that the block belongs to the file
	if (checkBlock(file, blockNumber))
	{
		//Get the index of the first block of the file
		int b = getFirstBlockNum(file);

		//Block we want to delete is a file's first block
		if (blockNumber == b)
		{
			//Go through ROOT directory
			for (int i = 0; i < filename.size(); i++)
			{
				//Found the file in our ROOT
				if (filename[i] == file)
				{
					//Set the new firstblock
					firstBlock[i] = fat[blockNumber];
					//Break out of the loop
					break;
				}
			}
		}
		//Block we want to delete is not the fist block of the file
		else
		{
			//Search for block that contains the index of the block we want to delete
			while (fat[b] != blockNumber)
			{
				//Increment through the list
				b = fat[b];
			}

			//We found the block that contains the index of the block we want to delete
			//Update the list of blocks that belong to the file
			//Block before the block we are deleting is linked to the block that
			//comes after the block that is being deleted
			fat[b] = fat[blockNumber];
		}

		//Update the free list in the FAT
		//Block we are deallocating now points to next free block in free list
		fat[blockNumber] = fat[0];
		//Update the beginning of the free list
		fat[0] = blockNumber;
	}
	//blockNumber does not belong to the file
	else
	{
		cout << "Block number does not belong to the file" << endl;
		return 0;
	}
}

//Testing purposes
void Filesys::printROOT()
{
	cout << "Size of filename and firstblock should be the same" << endl;
	cout << filename.size() << " " << firstBlock.size() << endl;
	cout << "Printing the ROOT" << endl;
	for (int i = 0; i < filename.size(); i++)
	{
		cout << "Index " << i << ": " << filename[i] << firstBlock[i] << endl;
	}
}

//Testing purposes
void Filesys::printFAT()
{
	cout << "Printing the ROOT" << endl;
	for (int i = 0; i < fat.size(); i++)
	{
		cout << "Index " << i << ": " << fat[i] << endl;
	}
}
