#pragma once
#include <iostream>
#include "AtomLine.h"

class Pdb {
public:
	// Constructor for creating object from file name and line type
	Pdb(std::string file_name);
	// Constructor for creating object from `AtomLine` vector
	Pdb(std::vector<AtomLine> al) { atom_lines = al; load_AA_seq(); };
	// Constructor for creating object from `AtomLine` vector and `name` string
	Pdb(std::string n, std::vector<AtomLine> al) { name = n; atom_lines = al; load_AA_seq(); };
	std::vector<AtomLine> get_atom_lines() { return atom_lines; };
	std::string get_name() { return name; };
	std::string get_AA_seq() { return AA_seq; };
	std::vector<unsigned int> get_CA_indices() { return CA_indices; }
	void write_fasta(std::string file_name);
	std::vector<double> get_mean_atom_coords();
	void write_file(std::string file_name);
	void append(Pdb appendee);
	std::vector<AtomLine> add_vector(std::vector<double> col_vector);
	std::vector<AtomLine> mult_matrix(std::vector<std::vector<double>> sq_matrix);
	std::vector<AtomLine> axis_rotate(std::vector<double> axis_vector, std::vector<double> point, double angle);
	//std::vector<AtomLine> align_by_first_CAs(Pdb alignee);

private:
	void load_AA_seq();
	void load_CA_indices();

	std::vector<AtomLine> atom_lines;
	std::string name;
	std::string AA_seq;
	std::vector<unsigned int> CA_indices;

	friend double get_rmsd(Pdb* p1, Pdb* p2);
	friend double get_tm_score(Pdb* p1, Pdb* p2);
};