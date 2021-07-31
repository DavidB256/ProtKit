#pragma once
#include <vector>
using namespace std;

class AtomLine {
public:
	AtomLine(string a, string aa, char c, unsigned int rn, vector<double> cs, double o, double bf) {
		atom_name = a;
		AA_residue = aa;
		chain = c;
		residue_number = rn;
		coordinates = cs;
		occupancy = o;
		b_factor = bf;
	}
	void add_vector(vector<double> col_vector) {
		for (int i = 0; i < 3; i++) {
			coordinates[i] += col_vector[i];
		}
	}
	void mult_matrix(vector<vector<double>> sq_matrix) {
		vector<double> temp_vector = coordinates;
		for (int i = 0; i < 3; i++) {
			coordinates[i] = (sq_matrix[i][0] * temp_vector[0]) +
				(sq_matrix[i][1] * temp_vector[1]) +
				(sq_matrix[i][2] * temp_vector[2]);
		}
	}

private:
	string atom_name;
	string AA_residue;
	vector<double> coordinates;
	char chain;
	unsigned int residue_number;
	double occupancy;
	double b_factor;

	friend class Pdb;
	friend class Fncs;
};