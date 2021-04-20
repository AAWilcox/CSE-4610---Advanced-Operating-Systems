/*******************************************************************************
* Name:			Alyssa Wilcox
* Id:			006861225
* Date:			March 18, 2021
* Class:		CSE 4610 - Advanced OS
* Assignment:	Project Part 2
* Filename:		Block.h
* Purpose:		This class is used to house one member function. This member
*				function breaks an ostringstream buffer into blockSized blocks.		
********************************************************************************/

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