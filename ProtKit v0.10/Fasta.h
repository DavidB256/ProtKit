#pragma once
#include "ProtFile.h"
using namespace std;

class Fasta : public ProtFile {
public:
	Fasta(string file_name);
	~Fasta();
	string get_name();
	string get_AA_seq();
	vector<string> get_file_lines();
	void load_AA_seq();
	void print_AA_seq();

private:
	vector<string> file_lines;
	string name = "";
	string AA_seq = "";
	bool AA_seq_loaded = false;
};