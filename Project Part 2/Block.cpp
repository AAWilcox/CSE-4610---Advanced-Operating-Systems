/*******************************************************************************
* Name:			Alyssa Wilcox
* Id:			006861225
* Date:			March 18, 2021
* Class:		CSE 4610 - Advanced OS
* Assignment:	Project Part 2
* Filename:		Block.cpp
* Purpose:		Contains the member functions of the class Block.
********************************************************************************/

#include"Block.h"

/*******************************************************************************
* Purpose:		Breaks an ostringstream buffer into blockSized blocks.
* Parameter(s):	buffer: the input buffer to break into blocks.
*				blockSize: the size of a block in the filesystem.
* Return:		A vector containing the input buffer broken into blocks.
********************************************************************************/
vector<string> Block::block(string buffer, int blockSize)
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
		//Jump blockSize characters and reading blockSize characters at a time
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