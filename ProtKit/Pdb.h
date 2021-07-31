#pragma once
#include <iostream>
#include "AtomLine.h"
#include "Vars.h"
#include "Fncs.h"
using namespace std;

class Pdb {
public:
	Pdb(string file_name);
	// Constructor for creating object from `AtomLine` vector
	Pdb(vector<AtomLine> al) { atom_lines = al; load_AA_seq(); };
	// Constructor for creating object from `AtomLine` vector and `name` string
	Pdb(string n, vector<AtomLine> al) { name = n; atom_lines = al; load_AA_seq(); };
	vector<AtomLine> get_atom_lines() { return atom_lines; };
	string get_name() { return name; };
	string get_AA_seq() { return AA_seq; };
	vector<unsigned int> get_CA_indices() { return CA_indices; }
	void write_fasta(string file_name);
	vector<double> get_mean_atom_coords();
	void write_file(string file_name);
	void append(Pdb appendee);
	vector<AtomLine> add_vector(vector<double> col_vector);
	vector<AtomLine> mult_matrix(vector<vector<double>> sq_matrix);
	vector<AtomLine> axis_rotate(vector<double> axis_vector, vector<double> point, double angle);
	//vector<AtomLine> align_by_first_CAs(Pdb alignee);

private:
	void load_AA_seq();
	void load_CA_indices();

	vector<AtomLine> atom_lines;
	string name;
	string AA_seq;
	vector<unsigned int> CA_indices;

	friend class Fncs;
};