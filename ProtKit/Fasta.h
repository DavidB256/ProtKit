#pragma once
#include "ProtFile.h"
using namespace std;

class Fasta : public ProtFile {
public:
	Fasta(string file_name) {
        name = file_name;

        string line;
        ifstream f(name);

        if (f) {
            while (getline(f, line)) {
                if (line.find(">") != 0 && line.find(";") != 0) {
                    AA_seq += line;
                }
            }
        }
        else {
            cout << "File not found. Empty file loaded." << endl;
        }
        f.close();

        cout << "Loaded .fasta file." << endl;
	}
	string get_name() { return name; };
	string get_AA_seq() { return AA_seq; };
	void print_AA_seq() { cout << AA_seq << endl << "Length: " << AA_seq.length() << endl; };
	void write_file(string file_name) { ofstream f(file_name); cout << ">" << name << endl << AA_seq; f.close(); };
	void append_file(Fasta file) { AA_seq += file.AA_seq; };

private:
	string name = "";
	string AA_seq = "";
};