/*******************************************************************************
* Name:         Alyssa Wilcox
* Id:           006861225
* Date:         March 18, 2021
* Class:        CSE 4610 - Advanced OS
* Assignment:   Project Part 1 and 2
* Filename:     main.cpp
* Project Pt 1: Implement a class, Sdisk, that wil be used to simulate a disk 
*               drive. The disk drive will have numberOfBlocks many blocks where 
*               each block has blockSize many bytes.
* Project Pt 2: Implement a simple file system. In particular, write the software 
*               which will handle dynamic file management. This part will require 
*               the implementation of the class Filesys along with member functions. 
*               FAT refers to the File Allocation Table, and ROOT refers to the 
*               Root Directory.
********************************************************************************/

#include"Sdisk.h"
#include"Filesys.h"
#include"Block.h"

int main()
{
    //Create the disk - 256 blocks, block size 128 bytes
    Sdisk disk1("disk1", 256, 128);
    //Create the file system
    Filesys fsys("disk1", 256, 128);
    //Block object
    Block blk;

    //Create two new files
    fsys.newFile("file1");
    fsys.newFile("file2");

    //Buffer for both files
    string bfile1;
    string bfile2;

    //Add 1024 "1"s to buffer for file1
    for (int i = 1; i <= 1024; i++)
    {
        bfile1 += "1";
    }

    //Break buffer into blocks
    vector<string> blocks = blk.block(bfile1, 128);

    int blocknumber = 0;

    //Add blocks to file1
    for (int i = 0; i < blocks.size(); i++)
    {
        //addBlock returns the block number that was allocated
        blocknumber = fsys.addBlock("file1", blocks[i]);
    }

    //Delete the first block of file1
    fsys.delBlock("file1", fsys.getFirstBlockNum("file1"));

    //Add 2048 "2"s to buffer for file2
    for (int i = 1; i <= 2048; i++)
    {
        bfile2 += "2";
    }

    //Break buffer into blocks
    blocks = blk.block(bfile2, 128);

    //Add blocks to file2
    for (int i = 0; i < blocks.size(); i++)
    {
        //addBlock returns the block number that was allocated
        blocknumber = fsys.addBlock("file2", blocks[i]);
    }

    //Delete the last block of file2
    fsys.delBlock("file2", blocknumber);

    return 0;
}