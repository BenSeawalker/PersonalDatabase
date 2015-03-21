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
#include <sstream>
using namespace std;

//forward declaration

//program
vector<map<string, string>> read_data(string _filename);
void save_data(string _filename, vector<map<string, string>> _profiles);
void print_data(vector<map<string, string>> _profiles);
void print_menu();
string profile_string(map<string, string> _profile);
bool sort_by_name(map<string, string> _i, map<string, string> _j);

void find_user_data(vector<map<string, string>> _profiles);
void get_input(string &_str);
void clean_input();
vector<string> tokenize_string(string _str);
string lowercase(string _str);
string uppercase(string _str);

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
				find_user_data(profiles);
			break;
			case 2:

			break;
			case 3:
				
			break;
			case 4:
				print_data(profiles);
			break;
			case 5:
				cout << endl << "Goodbye!" << endl;
			break;
			default:
				cout << endl << "invalid input" << endl;
			break;
		}

		clean_input();
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
	cout << "1. Find a person's information" << endl
		<< "2. Add a person to the database" << endl
		<< "3. Edit a person's information" << endl
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
	return _profile["lastname"] + " " + _profile["firstname"] + " " + _profile["phone"] + " " + _profile["bdate"];
}

bool sort_by_name(map<string, string> _i, map<string, string> _j)
{
	return ((_i["lastname"] + _i["firstname"]).compare((_j["lastname"] + _j["firstname"])) < 0);
}



//menu choices
void find_user_data(vector<map<string, string>> _profiles)
{
	cout << endl << "Search For: ";
	string search_name;
	get_input(search_name);

	vector<string> tokens = tokenize_string(search_name);

	vector<map<string, string>> results;
	for (int i = 0; i < _profiles.size(); i++)
	{
		string fname = _profiles[i]["firstname"];
		string lname = _profiles[i]["lastname"];

		bool lfound = false;
		bool ffound = false;
		for (int j = 0; j < tokens.size(); j++)
		{
			if (!lfound) lfound = (lowercase(lname).find(lowercase(tokens[j])) != string::npos);
			if (!ffound) ffound = (lowercase(fname).find(lowercase(tokens[j])) != string::npos);
		}

		if (tokens.size() > 1)
		{
			if (lfound && ffound) results.push_back(_profiles[i]);
		}
		else if (lfound || ffound) results.push_back(_profiles[i]);
	}

	int rsize = results.size();
	if (rsize > 1)
	{
		cout << "did you mean?" << endl;
		for (int i = 0; i < rsize; i++)
		{
			cout << i+1 << ". " << results[i]["lastname"] << " " << results[i]["firstname"] << endl;
		}
		cout << endl << "input: ";
		
		int choice;
		cin >> choice;
		choice = max(min(choice, rsize), 1) - 1;

		cout << endl << profile_string(results[choice]) << endl;
	}
	else if (rsize == 1)
	{
		cout << endl << profile_string(results[0]) << endl;
	}
	else
	{
		cout << endl << "No matches found." << endl;
	}
}

// ** UTILITY FUNCTIONS **

//strings
vector<string> tokenize_string(string _str)
{
	vector<string> tokens;
	string buffer;
	stringstream ss(_str);

	while (ss >> buffer)
		tokens.push_back(buffer);

	return tokens;
}

string lowercase(string _str)
{
	transform(_str.begin(), _str.end(), _str.begin(), ::towlower);
	return _str;
}
string uppercase(string _str)
{
	transform(_str.begin(), _str.end(), _str.begin(), ::towupper);
	return _str;
}

//input
void get_input(string &_str)
{
	cin.ignore(cin.rdbuf()->in_avail());
	getline(cin, _str);
	clean_input();
}

void clean_input()
{
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
}

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