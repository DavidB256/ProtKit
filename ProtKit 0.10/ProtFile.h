#pragma once
#include <unordered_map>
using namespace std;

// This is the generic parent class for "Fasta" and "Pdb"
class ProtFile {
public:
	ProtFile();
	ProtFile(string file_name);
	~ProtFile();
	virtual string get_name();
	virtual string get_AA_seq();
	virtual vector<string> get_file_lines();
	virtual void load_AA_seq();
	virtual void print_AA_seq();
	virtual void create_fasta(string file_name);
	virtual void prune(string mode);
	virtual void transform(string mode);

private:
	vector<string> file_lines;
	string name = "";
	string AA_seq = "";
};
