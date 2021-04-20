/*******************************************************************************
* Name:			Alyssa Wilcox
* Id:			006861225
* Date:			April 19, 2021
* Class:		CSE 4610 - Advanced OS
* Assignment:	Project Part 3
* Filename:		Shell.cpp
* Purpose:		Implement a shell that takes commands and operands. Use it to
*               interact with the filesystem.
********************************************************************************/

#include "Shell.h"
#include"Block.h"

//Block object
Block blk;

//Constructor
Shell::Shell(string diskName, int numberOfBlocks, int blockSize)
    : Filesys(diskName, numberOfBlocks, blockSize)
{
    //Stub function, it is not meant to do anything
}

/*******************************************************************************
* Purpose:		Adds a file and its contents to the filesystem.
* Parameter(s):	file: the name of the file to add to the filesystem.
*               buffer: the contents of the file to write to the disk.
* Return:		1 if successful, 0 otherwise.
********************************************************************************/
int Shell::add(string file, string buffer)
{
    //No filename or buffer contents given
    if (file.empty() || buffer.empty())
    {
        cout << "Could not recognize command" << endl;
        cout << "Format is: $add filename contents_of_file" << endl;
        return 0;
    }

    //Create a new file through Filesys's newFile function
    //Returns a code upon success(1)/failure(0)
    int code = newFile(file);

    //Error occurred
    if (code < 1)
    {
        cout << "Could not add file the the file system" << endl;
        return 0;
    }

    //Can add the file - need to block the buffer to be able to add it to disk
    
    //Block object
    Block blk;

    //Split buffer into blocks, store blocks into blocks vector
    //Uses Sdisk's getBlockSize()
    vector<string> blocks = blk.block(buffer, getBlockSize());

    //Add blocks to the disk
    for (int i = 0; i < blocks.size(); i++)
    {
        //Uses Filesys's addBlock function to add block of data
        addBlock(file, blocks[i]);
    }

    //Success
    return 1;
}

/*******************************************************************************
* Purpose:		Lists all of the files in the class Shell.
* Parameter(s):	None.
* Return:		1 if successful, 0 otherwise.
********************************************************************************/
int Shell::dir()
{
    //Vector that holds all of the valid file names in our file system
    vector<string> flist = ls();

    //Iterate through the vector
    for (int i = 0; i < flist.size(); i++)
    {
        //Output the file names
        cout << flist[i] << endl;
    }

    //Success
    return 1;
}

/*******************************************************************************
* Purpose:		Delets a file. Deletes every block that belongs to the file. 
                The blocks still exist, they just no longer belong to the file. 
                This also removes the file name from the ROOT.
* Parameter(s):	file: the file to delete.
* Return:		1 if successful, 0 otherwise.
********************************************************************************/
//Deletes/reset every block that belongs to the file
//Once all blocks deleted/reset, remove the filename from ROOT
int Shell::del(string file) 
{
    //No file given
    if (file.empty())
    {
        cout << "No filename given" << endl;
        return 0;
    }
    //File does not exist
    if (getFirstBlockNum(file) == -1)
    {
        cout << "Error" << endl;
        return 0;
    }

    //Go through all the blocks of the file
    while (getFirstBlockNum(file) != 0)
    {
        //Delete the block
        //Uses Filesys's delBlock function
        delBlock(file, getFirstBlockNum(file));
    }

    //Remove the file
    //Uses Filesys's rmfile
    return rmfile(file);

    //Success
    return 1;
}

/*******************************************************************************
* Purpose:		Lists the contents of a file.
* Parameter(s):	file: the file to read from.
* Return:		1 if successful, 0 otherwise.
********************************************************************************/
int Shell::type(string file)
{
    //No file given
    if (file.empty())
    {
        cout << "No filename given" << endl;
        return 0;
    }

    //Get the number of the first block belonging to the file
    //Uses Filesys's getFirstBlockNum function
    int block = getFirstBlockNum(file);
    
    //File does not exist
    if (block < 0)
    {
        cout << "Error" << endl;
        return 0;
    }
    //File exists, but is empty
    if (block == 0)
    {
        cout << "Empty File" << endl;
        return 1;
    }

    //Read contents of a file into a string buffer
    string buffer;
    //Used to read a single block
    string b;

    //When block == 0, that means we're at the end of the file
    while (block != 0)
    {
        //Clear the buffer
        b.clear();

        //Read the block
        //Uses Filesys's readBlock function
        readBlock(file, block, b);

        //Add contents of block to buffer
        buffer += b;

        //Get the next block to read
        //Uses Filesys's nextBlock function to get the next block number
        //after block
        block = nextBlock(file, block);
    }

    //Whole file is in the string buffer, output it
    cout << buffer << endl;

    //Success
    return 1;
}

/*******************************************************************************
* Purpose:		Lists the contents of a file.
* Parameter(s):	file: the file to read from.
* Return:		1 if successful, 0 otherwise.
********************************************************************************/
int Shell::copy(string file1, string file2)
{
    //No file1 or file2 name given
    if (file1.empty() || file2.empty())
    {
        cout << "Could not recognize command" << endl;
        cout << "Format is: $copy source_file destination_file" << endl;
        return 0;
    }

    //Get the number of the first block belonging to file1
    //Uses Filesys's getFirstBlockNum function
    int block = getFirstBlockNum(file1);
    
    //File does not exist
    if (block < 0)
    {
        cout << "Error" << endl;
        return 0;
    }
    //File exists, but is empty
    if (block == 0)
    {
        cout << "Empty File" << endl;
        return 1;
    }
    
    //file1 is good, we can move to creating file2

    //Create the second file through Filesys's newFile function
    //Returns a code upon success(1)/failure(0)
    int code = newFile(file2);

    //Error occurred
    if (code < 1)
    {
        cout << "Could not add file the the file system" << endl;
        return 0;
    }

    //Used to read a single block
    string b;

    //Read blocks from file1
    while (block != 0)
    {
        //Clear the buffer
        b.clear();

        //Read a block from file1
        //Uses Filesys's readBlock function
        readBlock(file1, block, b);

        //Add the contents of that block (stored in b) to file2
        //Uses Filesys's addBlock function
        addBlock(file2, b);

        //Get the next block to read
        //Uses Filesys's nextBlock function to get the next block number
        //after block
        block = nextBlock(file1, block);
    }
    
    //Success
    return 1;
}