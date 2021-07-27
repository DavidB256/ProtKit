#pragma once
#include "ProtFile.h"
#include "AtomLine.h"
#include "UtilityFunctions.h"
#include "Vars.h"
using namespace std;

class Pdb : public ProtFile {
public:
	Pdb(string file_name);
	// Constructor for creating object from `AtomLine` vector
	Pdb(vector<AtomLine> al) { atom_lines = al; load_AA_seq(); };
	// Constructor for creating object from `AtomLine` vector and `name` string
	Pdb(string n, vector<AtomLine> al) { name = n; atom_lines = al; load_AA_seq(); };
	vector<AtomLine> get_atom_lines() { return atom_lines; };
	string get_name() { return name; };
	string get_AA_seq() { return AA_seq; };
	void print_AA_seq() { cout << AA_seq << endl << "Length: " << AA_seq.length() << endl; };
	void write_fasta(string file_name);
	vector<double> get_mean_atom_coords();
	void write_file(string file_name);
	void append(Pdb appendee);
	vector<AtomLine> add_vector(vector<double> col_vector);
	vector<AtomLine> mult_matrix(vector<vector<double>> sq_matrix);
	vector<AtomLine> axis_rotate(vector<double> axis_vector, vector<double> point, double angle);
	vector<AtomLine> align_by_first_Cas(Pdb alignee);

private:
	void load_AA_seq();
	vector<unsigned int> get_first_n_alpha_Ca_indices(unsigned int n);

	vector<AtomLine> atom_lines;
	string name;
	string AA_seq;
};