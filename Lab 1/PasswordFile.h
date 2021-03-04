//Alyssa Wilcox
//February 3, 2021
//CSE 4610 - Advanced OS
//Lab 1
//PasswordFile.h
//Maintain a username-password system by storing the usernames
//and passwords in a file. The file will consist of a single
//username and password per line with a space in between

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

class PasswordFile
{
public:
	//Opens the file and reads the names/passwords in the vectors user and password
	PasswordFile(string filename);
	
	//This adds a new user/password to the vectors and writes the vectors to the file filename
	void addpw(string newuser, string newpassword);

	//Returns true if user exists and password matches
	bool checkpw(string user, string passwd);

	//Accessor
	void viewVectors();

private:
	//The file that contains the password information
	string filename;
	
	//The list of usernames
	vector<string> user;

	//The list of passwords
	vector<string> password;

	//Writes the user/password vectors to the password file
	void synch();
};
