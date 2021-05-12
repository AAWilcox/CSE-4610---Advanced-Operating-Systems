/*******************************************************************************
* Name:         Alyssa Wilcox
* Id:           006861225
* Date:         May 10, 2021
* Class:        CSE 4610 - Advanced OS
* Assignment:   Lab 7
* Filename:     main.cpp
* Description:  This main program inputs commands to the shell.
*               It inputs commands as : command op1 op2.
*               Reads in commands input from the keyboard and then passes
*               then to the Shell to execute the commands.
* Commands:     $add file - adds a file
                $del file - deletes a file.
*               $type file - lists the contents of a file.
*               $copy source destination - copies the source file to the
*               destination file.
*               $dir - lists all of the files in the filesystem.
********************************************************************************/

#include"Sdisk.h"
#include"Filesys.h"
#include"Block.h"
#include"Shell.h"
#include"Table.h"

int main()
{
    //Create a table (which then creates the Sdisk, Filesys, and Shell)
    Table table("disk1", 256, 128, "flatFile", "indexFile");
    //Build the table
    table.Build_Table("data.txt");

    //Commands and operands
    string s;
    string command = "go";
    //Operands
    string op1, op2;

    while (command != "quit")
    {
        //Clear command and operands
        command.clear();
        op1.clear();
        op2.clear();

        //Displays before command
        cout << "$";

        //Get the command + operands
        getline(cin, s);

        //Distinguish between command and operands
        int firstblank = s.find(' ');
        if (firstblank < s.length()) s[firstblank] = '#';
        int secondblank = s.find(' ');
        command = s.substr(0, firstblank);
        if (firstblank < s.length())
            op1 = s.substr(firstblank + 1, secondblank - firstblank - 1);
        if (secondblank < s.length())
            op2 = s.substr(secondblank + 1);

        //Lists all the files in the file system
        if (command == "dir")
        {
            table.dir();
            continue;
        }
        //Add a file to the filesystem
        else if (command == "add")
        {
            //The variable op1 is the file
            //The variable op2 is the contents of the file
            table.add(op1, op2);
            continue;
        }
        //Delete a file
        else if (command == "del")
        {
            //The variable op1 is the file
            table.del(op1);
            continue;
        }
        //Displays the contents of a file
        else if (command == "type")
        {
            //The variable op1 is the file
            table.type(op1);
            continue;
        }
        //Copies the contents of one file to another
        else if (command == "copy")
        {
            //The variable op1 is the source file and the variable op2 is the destination file.
            table.copy(op1, op2);
            continue;
        }

        //else if (command == "search")
        //{
            // This is the command for Project 4
            // The variable op1 is the date
        //}
        //Input is an unrecognized command
        else if (command != "quit")
        {
            cout << "Command not recognized" << endl;
        }
    }
    return 0;
}