#pragma once
#include "ProtFile.h"
#include "AtomLine.h"
using namespace std;

// Parent class of `Pdb` and `Fasta` classes containing generic methods 
class ProtFile {
public:
	ProtFile() { };
	ProtFile(string file_name) { name = file_name; cout << "File type not recognized." << endl; };
	ProtFile(vector<AtomLine>) { cout << "File type not recognized." << endl; }
	virtual vector<AtomLine> get_atom_lines() { cout << "Unable to get 'ATOM' lines for this file type." << endl; return vector<AtomLine>(); };
	virtual string get_name() { return name; };
	virtual string get_AA_seq() { cout << "Unable to get AA sequence for this file type." << endl; return string(); };
	virtual void print_AA_seq() { cout << "Unable to print AA sequence for this file type." << endl; };
	virtual void write_fasta(string file_name) { cout << "Unable to write .fasta file from this file type." << endl; };
	virtual vector<double> get_mean_atom_coords() { cout << "Unable to get mean atom coordinates for this file type." << endl; return vector<double>(3); };
	virtual void write_file(string file_name) { cout << "Unable to write this file type to a new file." << endl; };
	virtual void append(ProtFile appendee) { cout << "Unable to append to this file type." << endl; };
	virtual vector<AtomLine> add_vector(vector<double> col_vector) { cout << "Unable to translate this file type by a coordinate vector." << endl; return vector<AtomLine>(); };
	virtual vector<AtomLine> mult_matrix(vector<vector<double>> sq_matrix) { cout << "Unable to multiply this file type by a rotation matrix." << endl; return vector<AtomLine>(); };
	virtual vector<AtomLine> axis_rotate(vector<double> axis_vector, vector<double> point, double angle) { cout << "Unable to rotate this file type around an axis." << endl; return vector<AtomLine>(); };
	virtual vector<AtomLine> align_by_first_Cas(ProtFile alignee) { cout << "Unable to align this file type with another file." << endl; return vector<AtomLine>(); };

private:
	string name;
};