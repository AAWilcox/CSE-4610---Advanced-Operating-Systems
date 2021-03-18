<h1 align="center">Project Part 1</h1>

<h2>Assignment Description</h2>

The first part of this project requires that you implement a class that will be used to simulate a disk drive. The disk drive will have *numberOfBlocks* many blocks where each block has *blockSize* many bytes.

<h2>An Explanation of the Member Functions</h2>

- <strong>Sdisk(string diskName, int numberOfBlocks, int blockSize)</strong>
  - This constructor incorporates the creation of the disk with the "formatting" of the device. It accepts the integer values *numberOfBlocks*, *blockSize*, a string *diskName* and creates a Sdisk (software-disk). The Sdisk is a file of characters which we will manipulate as a raw hard disk drive. The function will check if the file *diskName* exists. If the file exists, it is opened and treated as a Sdisk with *numberOfBlocks* many blocks of size *blockSize*. If the file does not exist, the function will create a file called *diskName* which contains *numberOfBlocks***blockSize* many characters. This file is logically divided up into *numberOfBlocks* many blocks where each block has *blockSize* many characters.
- <strong>getBlock(int blockNumber, string& buffer)</strong>
  - Retreives block *blockNumber* from the disk and stores the data in the string *buffer*. It returns an error code of 1 if successful and 0 otherwise.
- <strong>putBlock(int blockNumber, string buffer)</strong>
  - Writes the string *buffer* to block *blockNumber*. It returns an error code of 1 if successful and 0 otherwise.
- <strong>getNumberOfBlocks()</strong>
  - Accessor function
  - Returns the number of blocks in Sdisk.
- <strong>getBlockSize()</strong>
  - Accessor function
  - Returns the size of the blocks in Sdisk.

<h2>Credits</h2>

- Project Part 1 Instructions: http://cse.csusb.edu/murphy/cse4610/hw1.html
