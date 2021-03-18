<h1 align="center">Project Part 1</h1>

<h2>Assignment Description</h2>

The second part of this project requires that you implement a simple file system. In particular, you are going to write the software which which will handle dynamic file management. This part of the project will require you to implement the class Filesys along with member functions. In the description below, FAT refers to the File Allocation Table and ROOT refers to the Root Directory.

<h2>An Explanation of the Member Functions</h2>

- <strong>Filesys(string diskName, int numberOfBlocks, int blockSize)</strong>
  - This constructor reads from the sdisk and either opens the existing file system on the disk or creates one for an empty disk. Recall the Sdisk is a file of characters which we will manipulate as a raw hard disk drive. This file is logically divided up into *numberOfBlocks* many blocks where each block has *blockSize* many characters. Information is first read from block 1 to determine if an existing file system is on the disk. If a filesystem exists, it is opened and made available. Otherwise, the file system is created.
  - The file system consists of two primary data objects. The directory is a file that consists of information about files and sub-directories. The root directory contains a list of file (and directory) names along with a block number of the first block in the file (or directory). (Of course, other information about the file such as creation date, ownership, permissions, etc. may also be maintained).
  - The FAT is an array of block numbers indexed one entry for every block. Every file in the file system is made up of blocks, and the component blocks are maintained as linked lists within the FAT. FAT[0], the entry for the first block of the FAT, is used as a pointer to the first free (unused) block in the file system.
  - Both FAT and ROOT are stored in memory AND on the disk. Any changes made to either structure in memory must also be immediately written to the disk.
- <strong>readFS()</strong>
  - This function is used by the constructor to read the filesystem from Sdisk if one already exists.
- <strong>buildFS()</strong>
  - This function is used by the constructor to build a filesystem on the Sdisk if one does not already exist.
- <strong>fssynch()</strong>
  - This function writes FAT and ROOT to the Sdisk. It should be used every time FAT and ROOT are modified.
- <strong>newFile(string file)</strong>
  - This function adds an entry for the string *file* in ROOT with an initial first block of 0 (empty). It returns error codes of 1 if successful and 0 otherwise (no room or file already exists).
- <strong>rmFile(string file)</strong>
  - This function removes the entry *file* from ROOT if the file is empty (first block is 0). It returns error codes of 1 if successful and 0 otherwise (not empty or file does not exist).
- <strong>getFirstBlockNum(string file)</strong>
  - This function returns the block number of the first block in *file*. It returns the error code of 1 if successful and 0 if the file does not exist.
- <strong>addBlock(string file, string block)</strong>
  - This function adds a block of data stored in the string buffer *block* to the end of *file* and returns the block number. It returns error code 0 if the file does not exist.
- <strong>delBlock(string file, int blockNumber)</strong>
  - This function removes block numbered *blockNumber* from *file* and returns an error code of 1 if successful and 0 otherwise.
- <strong>readBlock(string file, int blockNumber, string& buffer)</strong>
  - This function gets block numbered *blockNumber* from *file* and stores the data in the string buffer. It returns an error code of 1 if successful and 0 otherwise.
- <strong>writeBlock(string file, int blockNumber, string buffer)</strong>
  - This function writes the *buffer* to the block numbered *blockNumber* in *file*. It returns an appropriate error code.
- <strong>nextBlock(string file, int blockNumber)</strong>
  - This function returns the number of the block that follows *blockNumber* in *file*. It will return 0 if *blockNumber* is the last block and -1 if some other error has occurred (such as file is not in the root directory, or blocknumber is not a block in file).
- <strong>checkBlock(string file, int blockNumber)</strong>
  - This function checks if a block belongs to a file.


<h2>Credits</h2>

- Project Part 2 Instructions: http://cse.csusb.edu/murphy/cse4610/hw2-2015.html
