#pragma once
#include <string>
#include <vector>
#include "Pdb.h"
using namespace std;

class Fncs {
public:
	// Defined in IOFunctions.cpp
	static string convert_double_to_string_of_certain_length(double d, unsigned int len);
	static string get_nth_word_from_string(string s, unsigned int word_num);
	static char get_first_alphabet_character_from_string(string s);
	static string remove_spaces_from_string(string s);
	static string get_input(ifstream& f);
	static vector<unsigned int> get_longest_common_substring_indices(string s1, string s2);
	static bool get_whether_loaded_files_is_too_short(vector<Pdb*>& lf, unsigned int s);
	//Defined in VectorFunctions.cpp
	static vector<double> normalize_vector(vector<double> v);
	static vector<double> add_vectors(vector<double> v1, vector<double> v2);
	static vector<double> subtract_vectors(vector<double> v1, vector<double> v2);
	static double get_dot_product(vector<double> v1, vector<double> v2);
	static vector<double> get_projection(vector<double> v, vector<double> projectee);
	static double get_distance_squared(vector<double> v1, vector<double> v2);
	//Defined in PdbFunctions.cpp
	static double get_rmsd(Pdb* p1, Pdb* p2);
	static double get_tm_score(Pdb* p1, Pdb* p2);
};