/*******************************************************************************
* Name:			Alyssa Wilcox
* Id:			006861225
* Date:			April 19, 2021
* Class:		CSE 4610 - Advanced OS
* Assignment:	Project Part 3
* Filename:		Shell.h
* Purpose:		The third part of the project requires that you implement a
*				simple shell that uses your file system. This part of the
*				project will require you to implement the class Shell along
*				with member functions.
********************************************************************************/

//Prevent module conflict
#ifndef SHELL_H
#define SHELL_H

#include "Filesys.h"

//Shell class, inherits from Filesys
class Shell : public Filesys
{
public:
	//Constructor
	Shell(string fileName, int numberOfBlocks, int blockSize);
	//Lists all files in the file system
	int dir();
	//Add a new file and its contents using input from keyboard
	int add(string file, string buffer);
	//Deletes the file
	int del(string file);
	//Lists the contents of a file
	int type(string file);
	//Copies file1 to file2
	int copy(string file1, string file2);
};

#endif
