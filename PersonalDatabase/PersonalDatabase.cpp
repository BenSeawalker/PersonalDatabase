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

int find_user_data(vector<map<string, string>> _profiles);
void add_user(vector<map<string, string>>& _profiles);
void edit_user(vector<map<string, string>>& _profiles);

void get_input(string &_str);
string get_input();
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
				get_input();
			break;
			case 2:
				add_user(profiles);
				get_input();
			break;
			case 3:
				edit_user(profiles);
				get_input();
			break;
			case 4:
				print_data(profiles);
				get_input();
			break;
			case 5:
				cout << endl << "Goodbye!" << endl;
				get_input();
			break;
			default:
				cout << endl << "invalid input" << endl;
			break;
		}

		cout.flush();
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
	cout << endl << "--------------------------------------------------------------" << endl;
	cout << "\tPersonal Database" << endl << endl
		<< "1. Find a person's information" << endl
		<< "2. Add a person to the database" << endl
		<< "3. Edit a person's information" << endl
		<< "4. Display all profiles" << endl
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
int find_user_data(vector<map<string, string>> _profiles)
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
		int lpos = -1;
		for (int j = 0; j < tokens.size(); j++)
		{
			if (!lfound)
			{
				lfound = (lowercase(lname).find(lowercase(tokens[j])) == 0);
				if(lfound) lpos = j;
			}
			if (!ffound && j != lpos) ffound = (lowercase(fname).find(lowercase(tokens[j])) == 0);
		}


		if (tokens.size() > 1)
		{
			_profiles[i]["index"] = to_string(i);
			if (lfound && ffound) results.push_back(_profiles[i]);
		}
		else if (lfound || ffound) 
		{
			_profiles[i]["index"] = to_string(i);
			results.push_back(_profiles[i]);
		}
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
		return stoi(results[choice]["index"]);
	}
	else if (rsize == 1)
	{
		cout << endl << profile_string(results[0]) << endl;
		return stoi(results[0]["index"]);
	}
	else
	{
		cout << endl << "No matches found." << endl;
		return -1;
	}
}

void add_user(vector<map<string, string>>& _profiles)
{
	map<string, string> mp;
	clean_input();
	cout << "Enter last name: ";
	cin >> mp["lastname"];
	clean_input();

	cout << "Enter first name: ";
	cin >> mp["firstname"];
	clean_input();

	cout << "Enter phone number: ";
	cin >> mp["phone"];
	clean_input();

	cout << "Enter Birth Date (mm/dd/yyyy): ";
	cin >> mp["bdate"];
	clean_input();

	cout << endl << "New Profile: " << profile_string(mp) << endl;

	_profiles.push_back(mp);
	sort(_profiles.begin(), _profiles.end(), sort_by_name);
}

void edit_user(vector<map<string, string>>& _profiles)
{
	int index = find_user_data(_profiles);

	if (index > -1)
	{
		string lname = _profiles[index]["lastname"];
		string fname = _profiles[index]["firstname"];
		string phone = _profiles[index]["phone"];
		string bdate = _profiles[index]["bdate"];

		string input;
		cout << endl << "Edit Data or press enter to leave as is:" << endl << endl
			<< "Last Name (" + lname + ") : ";
		get_input(input);
		if (input.length() > 0)
			lname = input;

		cout << endl << "First Name (" + fname + ") : ";
		get_input(input);
		if (input.length() > 0)
			fname = input;

		cout << endl << "Phone Number (" + phone + ") : ";
		get_input(input);
		if (input.length() > 0)
			phone = input;

		cout << endl << "Birth Date (" + bdate + ") : ";
		get_input(input);
		if (input.length() > 0)
			bdate = input;

		cout << endl;

		_profiles[index]["lastname"] = lname;
		_profiles[index]["firstname"] = fname;
		_profiles[index]["phone"] = phone;
		_profiles[index]["bdate"] = bdate;

		cout << "Updated Info: " << profile_string(_profiles[index]) << endl;

		sort(_profiles.begin(), _profiles.end(), sort_by_name);
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
string get_input()
{
	string str;

	cin.ignore(cin.rdbuf()->in_avail());
	getline(cin, str);
	clean_input();

	return str;
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