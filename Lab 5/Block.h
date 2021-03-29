//Prevent module conflict
#ifndef BLOCK_H
#define BLOCK_H

#include<vector>
#include<string>
using namespace std;

class Block
{
public:
	//Breaks an ostringstream buffer into blocks
	vector<string> block(string buffer, int blockSize);
};
#endif

