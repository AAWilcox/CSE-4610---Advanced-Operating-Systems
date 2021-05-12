<h1 align="center">Project Part 4</h1>

<h2>Assignment Description</h2>

In this part of the project, you are going to create a database system with a single table which uses the file system from Project II. The input file will consist of records
associated with Art History.

<h2>An Explanation of the Member Functions</h2>

- <strong>Table(string diskName, int blockSize, int numberOfBlocks, string flatFile, string indexFile</strong>
  - This constructor creates the table object. It creates the new (empty) files flatfile and indexfile in the file system on the Sdisk using diskname.
- <strong>int Build_Table(string input_file)</strong>
  - This module will read records from the input file (the raw data file described above), add the records to the flatfile and create index records consisting of the date and block number, 
  and then add the index records to the index file. (Note that index records will have 10 bytes .. 5 bytes for the date and 5 bytes for the block number.)
- <strong>int IndexSearch(string value)</strong>
  - This module accepts a key value, and searches the index file indexfile for the record where the date matches the specified value. 
  IndexSearch then returns the block number key of the index record where the match occurs.
- <strong>int Search(string key)</strong>
  - This module accepts a key value, and searches the index file with a call to IndexSearch for the record where the date matches the specified value. 
  IndexSearch returns the blocknumber of the block in the flat file where the target record is located. This block should then be read and the record displayed.

<h2>Credits</h2>

- Project Part 4 Instructions: http://cse.csusb.edu/murphy/cse4610/hw4.html

