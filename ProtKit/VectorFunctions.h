#pragma once
#include <fstream>
#include <math.h>
#include <sstream>
#include <string>
using namespace std;

// Returns parameter `d` as a string of length `len`, 
// achieved by adding spaces added to the front, truncating, or neither
static string convert_double_to_string_of_certain_length(double d, unsigned int len) {
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
static string get_nth_word_from_string(string s, unsigned int word_num) {
	stringstream ss;
	string return_val;
	ss << s;
	for (unsigned int i = 0; i < word_num; i++)
		ss >> return_val;
	return return_val;
}

// Returns first character of string that is a letter of the alphabet
static char get_first_alphabet_character_from_string(string s) {
	for (char i : s) {
		if (isalpha(i))
			return i;
	}
	// Default to 'C' for carbon, but this will likely never get reached
	return 'C';
}

static string remove_spaces_from_string(string s) {
	string return_val = "";
	for (char i : s) {
		if (i != ' ')
			return_val += i;
	}
	return return_val;
}

// Returns normalized version of argument `v`
static vector<double> normalize_vector(vector<double> v) {
	double vector_norm = 0;
	for (double i : v)
		vector_norm += pow(i, 2);
	vector_norm = pow(vector_norm, 0.5);
	for (double& i : v)
		i /= vector_norm;

	return v;
}

// Gets user input from a script file if one is open, from `cin` otherwise
static string get_input(ifstream& f) {
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
static bool get_whether_loaded_files_is_too_short(vector<Pdb*>& lf, unsigned int s) {
	if (lf.size() < s) {
		cout << "This command can only be called if at least " << s << " files have been loaded, but only " << lf.size() << " files have been loaded." << endl;
		return true;
	}
}

// Returns the vector achieved by element-wise addition of `v1` and `v2`
static vector<double> add_vectors(vector<double> v1, vector<double> v2) {
	for (unsigned int i = 0; i < v1.size(); i++)
		v1[i] += v2[i];
	return v1;
}

// Returns the vector achieved by element-wise subtraction of `v2` from `v1`
static vector<double> subtract_vectors(vector<double> v1, vector<double> v2) {
	for (unsigned int i = 0; i < v1.size(); i++)
		v1[i] -= v2[i];

	return v1;
}

// Returns the dot product of `v1` and `v2`
static double get_dot_product(vector<double> v1, vector<double> v2) {
	double dot_product = 0;
	for (int i = 0; i < 3; i++) {
		dot_product += v1[i] * v2[i];
	}

	return dot_product;
}

// Returns projection of `projectee` onto `v`
static vector<double> get_projection(vector<double> v, vector<double> projectee) {
	double scalar =
		get_dot_product(v, projectee) / get_dot_product(v, v);
	for (double& i : v)
		i *= scalar;

	return v;
}

// Returns 3-vector of unsigned ints whose entries represent
// starting index of the longest common substring (LCS) in the `s1` argument,
// starting index of the LCS in the `s2` argument, and
// the length of the LCS, respectively 
static vector<unsigned int> get_longest_common_substring_indices(string s1, string s2) {
	// Guarantee that `s1` is not longer than `s2`
	bool swapped = false;
	if (s2.length() < s1.length()) {
		string temp_string = s1;
		s1 = s2;
		s2 = temp_string;
		swapped = true;
	}

	unsigned int find_pos;

	for (unsigned int i = s1.length(); i > 0; i--) {
		for (unsigned int j = 0; j <= s1.length() - i; j++) {
			find_pos = s2.find(s1.substr(j, i));
			if (find_pos != string::npos) {
				if (swapped)
					return vector<unsigned int>{find_pos, j, i};
				else
					return vector<unsigned int>{j, find_pos, i};
			}
		}
	}

	// If no characters are shared between `s1` and `s2`
	cout << "No common substring found. Returning 3-vector of zeros." << endl;
	return vector<unsigned int>(3, 0);
}

// Returns square of distance between points represented by vectors `v1` and `v2`
static double get_distance_squared(vector<double> v1, vector<double> v2) {
	double distance_squared = 0;
	for (unsigned int i = 0; i < v1.size(); i++)
		distance_squared += pow(v1[i] - v2[i], 2);
	return distance_squared;
}