#pragma once
#include <vector>
#include <fstream>
#include "Pdb.h"

namespace fncs {
	// Defined in IOFunctions.cpp
	std::string convert_double_to_string_of_certain_length(double d, unsigned int len);
	std::string get_nth_word_from_string(std::string s, unsigned int word_num);
	char get_first_alphabet_character_from_string(std::string s);
	std::string remove_spaces_from_string(std::string s);
	std::string get_input(std::ifstream& f);
	std::vector<unsigned int> get_longest_common_substring_indices(std::string s1, std::string s2);
	bool get_whether_loaded_files_is_too_short(std::vector<Pdb*>& lf, unsigned int s);
	//Defined in VectorFunctions.cpp
	std::vector<double> normalize_vector(std::vector<double> v);
	std::vector<double> add_vectors(std::vector<double> v1, std::vector<double> v2);
	std::vector<double> subtract_vectors(std::vector<double> v1, std::vector<double> v2);
	double get_dot_product(std::vector<double> v1, std::vector<double> v2);
	std::vector<double> get_projection(std::vector<double> v, std::vector<double> projectee);
	double get_distance_squared(std::vector<double> v1, std::vector<double> v2);
}