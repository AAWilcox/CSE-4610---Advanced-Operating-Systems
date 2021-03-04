//Alyssa Wilcox
//March 3, 2021
//CSE 4610 - Advanced OS
//Lab 2
//main.cpp

#include "Sdisk.h"

int main()
{
	//Create our sdisk, 16 blocks, 32 bytes per block
	Sdisk disk1("test1.txt", 16, 32);

	//Strings for our blocks
	string block1, block2, block3, block4;

	//Fill block 1 and 2
	for (int i = 1; i <= 32; i++) block1 = block1 + "1";
	for (int i = 1; i <= 32; i++) block2 = block2 + "2";

	//Write string block1 to block 4
	disk1.putBlock(4, block1);
	//Get what's in block 4, save it to string block3
	disk1.getBlock(4, block3);
	cout << "Should be 32 1s : ";
	cout << block3 << endl;

	//Write string block2 to block 8
	disk1.putBlock(8, block2);
	//Get what's in block 8, save it to string block4
	disk1.getBlock(8, block4);
	cout << "Should be 32 2s : ";
	cout << block4 << endl;

	return 0;
}