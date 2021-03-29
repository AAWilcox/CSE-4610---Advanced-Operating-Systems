#include"Block.h"

//Block function - breaks an ostringstream buffer into blocks
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