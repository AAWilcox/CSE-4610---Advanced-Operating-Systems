//Alyssa Wilcox
//February 3, 2021
//CSE 4610 - Advanced OS
//Lab 1
//PasswordFile.cpp
//Contains the functionalities of the PasswordFile class

#include "PasswordFile.h"

//Constructor
PasswordFile::PasswordFile(string filename)
{
	//Strings to hold data being read
	string u, p;
	//input file string object
	ifstream infile;
	//Save the file name, convert it to C string
	this->filename = filename;
	//Open the file
	infile.open(filename.c_str());
	//Read the first entry
	infile >> u >> p;
	//While we can open/read the file
	//and while we are not at the end
	while (infile.good())
	{
		//Process data that was read
		user.push_back(u);
		password.push_back(p);
		//Read next entry
		infile >> u >> p;
	}
	//Close the file
	infile.close();
}

//Adds a new user/password
void PasswordFile::addpw(string newuser, string newpassword)
{
	//Do not add a user that already exists
	for (int i = 0; i < user.size(); i++)
	{
		if (newuser == user[i])
		{
			cout << "User exists" << endl;
			return;
		}
	}

	//Add new user and password onto the vectors
	user.push_back(newuser);
	password.push_back(newpassword);

	//But now vectors and "password.txt" out of synch
	//Call synch()
	synch();
}

//Writes the user/password vectors to the password file
void PasswordFile::synch()
{
	//Output filestream object
	ofstream outfile;
	//Open the file
	outfile.open(filename.c_str());
	//Write usernames and passwords to file
	for (int i = 0; i < user.size(); i++)
	{
		outfile << user[i] << " " << password[i]
			<< " " << endl;
	}
	//Close the file
	outfile.close();
}

//Check if user exists and password matches
bool PasswordFile::checkpw(string user, string passwd)
{
	//Search for the user - linear search
	for (int i = 0; i < this->user.size(); i++) 
	{
		//User was found
		if (this->user[i] == user) 
		{
			cout << "User exists" << endl;
			//Check password
			if (passwd == password[i]) 
			{
				//Password matches
				return true;
			}
			else
			{
				//Password does not match
				cout << "Password does not match" << endl;
				return false;
			}	
		}
		else
		{
			cout << "User does not exist" << endl;
			return false;
		}
	}
}

//View what's in the vectors
void PasswordFile::viewVectors()
{
	cout << "User vectors: " << endl;
	for (int i = 0; i < user.size(); i++) {
		cout << user[i] << endl;
	}

	cout << endl;

	cout << "Password vector: " << endl;
	for (int i = 0; i < password.size(); i++) {
		cout << password[i] << endl;
	}

	cout << endl;
}
