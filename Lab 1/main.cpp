//Alyssa Wilcox
//February 3, 2021
//CSE 4610 - Advanced OS
//Lab 1
//main.cpp

#include "PasswordFile.h"

int main()
{
	//Create PasswordFile object
	//Class's constructor is called, password.txt is passed to constructor
	PasswordFile passfile("password.txt");

	passfile.viewVectors();

	//Add new passwords
	passfile.addpw("dbotting", "123qwe");
	passfile.viewVectors();
	passfile.addpw("egomez", "qwerty");
	passfile.viewVectors();
	passfile.addpw("tongyu", "liberty");
	passfile.viewVectors();

	//Check if password matches
	bool matches;
	matches = passfile.checkpw("dbotting", "123qwet");
	if (matches == true) {
		cout << "The password matches!" << endl;
	}
	else
		cout << "The password does not match" << endl;

	return 0;
}