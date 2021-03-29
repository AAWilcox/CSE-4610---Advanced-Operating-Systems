/*******************************************************************************
* Name:			Alyssa Wilcox
* Id:			006861225
* Date:			March 18, 2021
* Class:		CSE 4610 - Advanced OS
* Assignment:	Project Part 2
* Filename:		Filesys.cpp
* Purpose:		Contains the member functions of the Filsys class.
********************************************************************************/

#include"Filesys.h"
#include"Block.h"
#include<sstream>

//Block object
Block blk;

/*******************************************************************************
* Purpose:		The constructor function for the class Filesys. Calls Sdisk's
*				constructor explicitly.
* Parameter(s):	diskName: the name of the disk.
*				numberOfBlocks: the number of blocks in the disk.
*				blockSize: the size of the blocks in the disk.
* Return:		None.
********************************************************************************/
Filesys::Filesys(string diskName, int numberOfBlocks, int blockSize)
	: Sdisk(diskName, numberOfBlocks, blockSize)
{
	//Every file in the root has 10 characters
	//rootSize is the maximum number of files we can have
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

/*******************************************************************************
* Purpose:		A function used by the constructor to build a file system if
*				one does not already exist on the disk.
* Parameter(s):	None.
* Return:		None.
********************************************************************************/
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

/*******************************************************************************
* Purpose:		A function used by the constructor to read from an already
*				existing file system on the disk.
* Parameter(s):	None.
* Return:		None.
********************************************************************************/
void Filesys::readFS()
{
	//=-=-=-=-= READ IN THE ROOT =-=-=-=-=

	//Input buffer used to read from the disk
	istringstream istream;

	//Buffer to hold ROOT
	string buffer;

	//Get the ROOT, which is in block 1 of Sdisk, write to buffer
	getBlock(1, buffer);

	//Load buffer into istream, convert to string
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

	//Load buffer into istream2, convert to string
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

/*******************************************************************************
* Purpose:		Writes FAT and ROOT to the Sdisk. It should be used every time
*				FAT and ROOT are modified.
* Parameter(s):	None.
* Return:		None.
********************************************************************************/
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
	vector<string> blocks = blk.block(buffer, getBlockSize());

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

	//Split our buffer2 string into blocks so we can write it to Sdisk
	vector<string> blocks2 = blk.block(buffer2, getBlockSize());

	//Write blocks2 (our FAT split into blocks) to Sdisk
	for (int i = 0; i < blocks2.size(); i++)
	{
		//FAT begins in block 2 of Sdisk
		putBlock(2 + i, blocks2[i]);
	}
}

/*******************************************************************************
* Purpose:		Writes the FAT and ROOT (closing the disk).
* Parameter(s):	None.
* Return:		1 once the FAT and ROOT have been written to the Sdisk.
********************************************************************************/
//Write FAT and ROOT to the Sdisk (closing the disk)
int Filesys::fsclose()
{
	fssynch();
	return 1;
}

/*******************************************************************************
* Purpose:		Adds an entry for the string file in ROOT with an initial first
*				block of 0 (empty).
* Parameter(s):	file: the name of the new file/entry.
* Return:		1 if the new entry is successfully added.
*				0 otherwise (no room in ROOT directory, file already exists).
********************************************************************************/
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

/*******************************************************************************
* Purpose:		Removes the entry file from ROOT if the file is empty (first
*				block is 0).
* Parameter(s):	file: the entry to remove from the ROOT.
* Return:		1 if the entry is successfully removed.
*				0 otherwise (file does not exist, file is not empty).
********************************************************************************/
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
				filename[i] = "xxxx";
				//Synch filename vector to Sdisk
				fssynch();
				//Success
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

/*******************************************************************************
* Purpose:		Finds the block number of the first block in a file.
* Parameter(s):	file: the file to find the first block of.
* Return:		The block number of the first block.
*				-1 if the file does not exist.
********************************************************************************/
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

/*******************************************************************************
* Purpose:		Adds a block of data stored in the string buffer to the end of
*				the file.
* Parameter(s):	file: the file to add a block of data to.
*				buffer: the block of data to write to the file.
* Return:		The number of the block allocated to the file.
*				0 otherwise (if the file does not exist, if the buffer size does
*				not match the block size).
********************************************************************************/
int Filesys::addBlock(string file, string buffer)
{
	//Check to make sure the block we want to add is the correct block size
	if (buffer.length() == getBlockSize())
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
				//We found the file's old last block. Update it
				fat[b] = allocate;
			}

			//Write new contents into the allocated block
			putBlock(allocate, buffer);

			//Write the new ROOT/FAT to Sdisk
			fssynch();

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

/*******************************************************************************
* Purpose:		Removes a block numbered blockNumber from a file.
* Parameter(s):	file: the file to remove a block from.
*				blockNumber: the block to remove from a file.
* Return:		1 if the block is successfully removed from the file.
*				0 otherwise (block does not belong to the file).
********************************************************************************/
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

		//Write updated ROOT and FAT to disk
		fssynch();

		//Success
		return 1;
	}
	//blockNumber does not belong to the file
	else
	{
		cout << "Block number does not belong to the file" << endl;
		return 0;
	}
}

/*******************************************************************************
* Purpose:		Gets block numbered blockNumber from the file and stores the
*				data in the string buffer.
* Parameter(s):	file: the file to read from.
*				blockNumber: the block to read from.
*				buffer: the string buffer used to write a block's content to.
* Return:		1 if the block is successfully read.
*				0 otherwise (block does not belong to the file).
********************************************************************************/
int Filesys::readBlock(string file, int blockNumber, string& buffer)
{
	//Check if the blockNumber belongs to the file
	if (checkBlock(file, blockNumber))
	{
		//Read the block
		getBlock(blockNumber, buffer);
		//Success
		return 1;
	}
	//blockNumber does not belong to the file
	else
	{
		cout << "Block number does not belong to the file" << endl;
		return 0;
	}
}

/*******************************************************************************
* Purpose:		Writes the string buffer to the block numbered blockNumber
*				in a file.
* Parameter(s):	file: the file to write to.
*				blockNumber: the number of the block to write to.
*				buffer: the string input to write to a block.
* Return:		1 if the block is successfully written to.
*				0 otherwise (the block does not belong to the file).
********************************************************************************/
int Filesys::writeBlock(string file, int blockNumber, string buffer)
{
	//Check if the blockNumber belongs to the file
	if (checkBlock(file, blockNumber))
	{
		//Write to the block
		putBlock(blockNumber, buffer);
		//Success
		return 1;
	}
	//blockNumber does not belong to the file
	else
	{
		cout << "Block number does not belong to the file" << endl;
		return 0;
	}
}

/*******************************************************************************
* Purpose:		Finds the number of the block that follows blockNumber in a file.
* Parameter(s):	file: the file to find the next block of.
*				blockNumber: the block to find the next block of.
* Return:		block number of the next block	
*				0 if blockNumber is the last block of the file.
*				-1 otherwise (block number does not belong to the file).
********************************************************************************/
int Filesys::nextBlock(string file, int blockNumber)
{
	//Check if the blockNumber belongs to the file
	if (checkBlock(file, blockNumber))
	{
		//If blockNumber is the last block
		if (fat[blockNumber] == 0)
		{
			cout << blockNumber << " is the last block" << endl;
			return 0;
		}
		//blockNumber is not the last block
		else
			//Return index of next block
			return fat[blockNumber];
	}
	//blockNumber does not belong to the file
	else
	{
		cout << "Block number does not belong to the file" << endl;
		return -1;
	}
}

/*******************************************************************************
* Purpose:		Checks if blockNumber belongs to a file.
* Parameter(s):	file: the file to check if a blockNumber belongs to it.
*				blockNumber: the block to check if it belongs to a file or not.
* Return:		true if the block belongs to the file.
*				false if the block does not belong to the file.
********************************************************************************/
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

	//b is the last block of the file
	if (b == 0)
		return true;

	//Block does not belong to the file
	return false;
}