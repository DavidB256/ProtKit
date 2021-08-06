#include <iostream>
#include <sstream>
#include <fstream>
#include "Fncs.h"
using namespace std;

// Returns parameter `d` as a string of length `len`, 
// achieved by adding spaces added to the front, truncating, or neither
string fncs::convert_double_to_string_of_certain_length(double d, unsigned int len) {
	string return_val = to_string(d);
	if (return_val.length() < len) {
		return string(len - return_val.length(), ' ') + return_val;
	}
	else if (return_val.length() > len) {
		return return_val.substr(0, len);
	}
	return return_val;
}

// Returns the nth (1-based indexing) space-separated word from a string using `stringstream`
string fncs::get_nth_word_from_string(string s, unsigned int word_num) {
	stringstream ss;
	string return_val;
	ss << s;
	for (unsigned int i = 0; i < word_num; i++)
		ss >> return_val;
	return return_val;
}

// Returns first character of string that is a letter of the alphabet
char fncs::get_first_alphabet_character_from_string(string s) {
	for (char i : s) {
		if (isalpha(i))
			return i;
	}
	// Default to 'C' for carbon, but this will likely never get reached
	return 'C';
}

string fncs::remove_spaces_from_string(string s) {
	string return_val = "";
	for (char i : s) {
		if (i != ' ')
			return_val += i;
	}
	return return_val;
}

// Gets user input from a script file if one is open, from `cin` otherwise
string fncs::get_input(ifstream& f) {
	string line;

	// Get input from file if one is open, from user otherwise
	if (f.is_open()) {
		getline(f, line);
		cout << line << endl;
	}
	else {
		getline(cin, line);
	}
	// Stop reading from file upon reaching its end
	if (f.eof())
		f.close();

	return line;
}

// Checks whether `s` files have been loaded, where `s` is the necessary amount for a command
bool fncs::get_whether_loaded_files_is_too_short(vector<Pdb*>& lf, unsigned int s) {
	if (lf.size() < s) {
		cout << "This command can only be called if at least " << s << " files have been loaded, but only " << lf.size() << " files have been loaded." << endl;
		return true;
	}
	return false;
}