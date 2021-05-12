/*******************************************************************************
* Name:			Alyssa Wilcox
* Id:			006861225
* Date:			April 19, 2021
* Class:		CSE 4610 - Advanced OS
* Assignment:	Lab 6
* Filename:		Shell.h
* Purpose:		Implement a shell that takes commands and operands. Use it to
*               interact with the filesystem. All functions return 1 if
*				successful and 0 otherwise. Only use functions from filesys
*				to implement shell functions, not Sdisk.
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
	//Searches the filesystem for a record using a key
	//int search(string key);
};

#endif
