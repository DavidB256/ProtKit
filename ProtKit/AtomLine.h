#pragma once
#include <vector>

class AtomLine {
public:
	AtomLine(std::string a, std::string aa, char c, unsigned int rn, std::vector<double> cs, double o, double bf) {
		atom_name = a;
		AA_residue = aa;
		chain = c;
		residue_number = rn;
		coordinates = cs;
		occupancy = o;
		b_factor = bf;
	}

	std::vector<double> get_coordinates() { return coordinates; };

	void add_vector(std::vector<double> col_vector) {
		for (int i = 0; i < 3; i++)
			coordinates[i] += col_vector[i];
	}
	void mult_matrix(std::vector<std::vector<double>> sq_matrix) {
		std::vector<double> temp_vector = coordinates;
		for (int i = 0; i < 3; i++) {
			coordinates[i] = (sq_matrix[i][0] * temp_vector[0]) +
				(sq_matrix[i][1] * temp_vector[1]) +
				(sq_matrix[i][2] * temp_vector[2]);
		}
	}

private:
	std::string atom_name;
	std::string AA_residue;
	std::vector<double> coordinates;
	char chain;
	unsigned int residue_number;
	double occupancy;
	double b_factor;

	friend class Pdb;
};