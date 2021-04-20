<h1 align="center">Project Part 3</h1>

<h2>Assignment Description</h2>

The second part of this project requires that you implement a simple shell that uses your file system. This part of the project will require you to implement the class
<strong>Shell</strong> along with member functions.

<h2>An Explanation of the Member Functions</h2>

- <strong>Shell(string filename, int blockSize, int numberOfBlocks)</strong>
  - This will create a shell object using the Filesys on the file filename.
- <strong>int dir()</strong>
  - This will list all the files in the root directory.
- <strong>int add(string file, string buffer)</strong>
  - This will add a new file and its contents using input from the keyboard.
- <strong>int del(string file)</strong>
  - This deletes a file.
- <strong>int type(string file)</strong>
  - This lists the contents of a file.
- <strong>int copy(string file1, string file2)</strong>
  - This copies the contents of file1 to file2.
- <strong>int ls()</strong>
  - This function should be added to Filesys. It finds all of the valid filenames in the filesystem and adds them to a vector.


<h2>Credits</h2>

- Project Part 3 Instructions: http://cse.csusb.edu/murphy/cse4610/hw3.html

