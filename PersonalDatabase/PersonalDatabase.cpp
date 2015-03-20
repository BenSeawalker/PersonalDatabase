// PersonalDatabase.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

//forward declaration

//program
vector<map<string, string>> read_data(string _filename);
void save_data(string _filename, vector<map<string, string>> _profiles);
void print_data(vector<map<string, string>> _profiles);
void print_menu();
string profile_string(map<string, string> _profile);
bool sort_by_name(map<string, string> _i, map<string, string> _j);

//utility
ifstream file_open_read(string _filepath);
ofstream file_open_write(string _filepath);
ofstream file_open_append(string _filepath);
bool file_close(ofstream &_file);
bool file_close(ifstream &_file);




//CONSTANTS
//const int		MAX_CHARS = 15;
const string	FILE_NAME = "profiles.dtb";


int _tmain(int argc, _TCHAR* argv[])
{
	/*
	ofstream file = file_open_write(FILE_NAME);
	if (file.is_open())
	{
		file << "Fleischer Garrett 5309383912 05/07/1994" << endl
			<< "Fleischer Geneva 5309383912 07/24/1994" << endl
			<< "Meadows Stacy 5309383912 07/24/1994" << endl
			<< "Greenspan Chet 5309383912 07/24/1994" << endl
			<< "Shapiro Cam 5309383912 07/24/1994" << endl
			<< "Nicholson Ritchie 5309383912 07/24/1994" << endl
			<< "Clive Adams 5309383912 07/24/1994" << endl
			<< "Fleischer Corinne 5309383912 05/07/1994";
	}
	file_close(file);
	*/
	vector<map<string, string>> profiles = read_data(FILE_NAME);

	int input = 0;
	while (input != 5)
	{
		print_menu();
		cin >> input;
		switch (input)
		{
			case 1:

			break;
			case 2:

			break;
			case 3:
				
			break;
			case 4:
				print_data(profiles);
			break;
		}
	}

	save_data(FILE_NAME, profiles);

	return 0;
}


// ** PROGRAM FUNCTIONS **

//print
void print_data(vector<map<string, string>> _profiles)
{
	cout << "--------------------------------------------------------------" << endl;
	for (int i = 0; i < _profiles.size(); i++)
	{
		cout << profile_string(_profiles[i]) << endl;
	}
}

void print_menu()
{
	cout << "--------------------------------------------------------------" << endl;
	cout << "1. Find a persion's information" << endl
		<< "2. Add a person to the database" << endl
		<< "3. Edit a persion's information" << endl
		<< "4. Display all records" << endl
		<< "5. Quit" << endl << endl
		<< "input: ";
}

//file data
vector<map<string, string>> read_data(string _filename) //char _fname[][MAX_CHARS], char _lname[][MAX_CHARS], char _phone[][MAX_CHARS], char _bdate[][MAX_CHARS])
{
	vector<map<string, string>> profiles;

	ifstream file = file_open_read(_filename);
	if (file.is_open())
	{
		do
		{
			map<string, string> mp;
			file >> mp["lastname"] >> mp["firstname"] >> mp["phone"] >> mp["bdate"];

			//ignore blank lines
			if (mp["lastname"].compare("") != 0)
				profiles.push_back(mp);
			//_file >> _profiles[fsize]["lastname"] >> _profiles[fsize]["firstname"] >> _profiles[fsize]["phone"] >> _profiles[fsize]["bdate"];
			//_file >> _fname[fsize] >> _lname[fsize] >> _phone[fsize] >> _bdate[fsize];
		} while (!file.eof());
	}
	file_close(file);

	//sort imported data
	sort(profiles.begin(), profiles.end(), sort_by_name);

	return profiles;
}

void save_data(string _filename, vector<map<string, string>> _profiles)//char _fname[][MAX_CHARS], char _lname[][MAX_CHARS], char _phone[][MAX_CHARS], char _bdate[][MAX_CHARS])
{
	ofstream file = file_open_write(_filename);
	if (file.is_open())
	{
		for (int i = 0; i < _profiles.size(); i++)
		{
			file << profile_string(_profiles[i]);
			//don't add a new line at end of file.
			if (i < _profiles.size() - 1)
				file << endl;
		}
	}
	file_close(file);
}



//data
string profile_string(map<string, string> _profile)
{
	return _profile["lastname"] + "\t" + _profile["firstname"] + "\t" + _profile["phone"] + "\t" + _profile["bdate"];
}

bool sort_by_name(map<string, string> _i, map<string, string> _j)
{
	return ((_i["lastname"] + _i["firstname"]).compare((_j["lastname"] + _j["firstname"])) < 0);
}


// ** UTILITY FUNCTIONS **


//file manipulation
ifstream file_open_read(string _filepath)
{
	ifstream fdata(_filepath);

	if (fdata.fail())
		cout << "Failed to open file for reading";

	return fdata;
}

ofstream file_open_write(string _filepath)
{
	ofstream fdata(_filepath);

	if (fdata.fail())
		cout << "Failed to open file for writing";

	return fdata;
}

ofstream file_open_append(string _filepath)
{
	ofstream fdata(_filepath, ios::app);

	if (fdata.fail())
		cout << "Failed to open file for appending";

	return fdata;
}

bool file_close(ofstream &_file)
{
	if (_file.is_open())
	{
		_file.close();
		return true;
	}

	return false;
}
bool file_close(ifstream &_file)
{
	if (_file.is_open())
	{
		_file.close();
		return true;
	}

	return false;
}