#pragma once
#include "ProtFile.h"
using namespace std;

class Pdb : public ProtFile {
public:
	Pdb(string file_name);
	~Pdb();
	string get_name();
	string get_AA_seq();
	vector<string> get_file_lines();
	void load_AA_seq();
	void print_AA_seq();
	void create_fasta(string file_name);
	void prune(string mode);

private:
	vector<string> file_lines;
	string name = "";
	string AA_seq = "";
	bool AA_seq_loaded = false;
};