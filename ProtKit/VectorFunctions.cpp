#include <math.h>
#include <iostream>
#include "Fncs.h"
using namespace std;

// Returns normalized version of argument `v`
vector<double> Fncs::normalize_vector(vector<double> v) {
	double vector_norm = 0;
	for (double i : v)
		vector_norm += pow(i, 2);
	vector_norm = pow(vector_norm, 0.5);
	for (double& i : v)
		i /= vector_norm;

	return v;
}

// Returns the vector achieved by element-wise addition of `v1` and `v2`
vector<double> Fncs::add_vectors(vector<double> v1, vector<double> v2) {
	for (unsigned int i = 0; i < v1.size(); i++)
		v1[i] += v2[i];
	return v1;
}

// Returns the vector achieved by element-wise subtraction of `v2` from `v1`
vector<double> Fncs::subtract_vectors(vector<double> v1, vector<double> v2) {
	for (unsigned int i = 0; i < v1.size(); i++)
		v1[i] -= v2[i];

	return v1;
}

// Returns the dot product of `v1` and `v2`
double Fncs::get_dot_product(vector<double> v1, vector<double> v2) {
	double dot_product = 0;
	for (int i = 0; i < 3; i++) {
		dot_product += v1[i] * v2[i];
	}

	return dot_product;
}

// Returns projection of `projectee` onto `v`
vector<double> Fncs::get_projection(vector<double> v, vector<double> projectee) {
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
vector<unsigned int> Fncs::get_longest_common_substring_indices(string s1, string s2) {
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
double Fncs::get_distance_squared(vector<double> v1, vector<double> v2) {
	double distance_squared = 0;
	for (unsigned int i = 0; i < v1.size(); i++)
		distance_squared += pow(v1[i] - v2[i], 2);
	return distance_squared;
}