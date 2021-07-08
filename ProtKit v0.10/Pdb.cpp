#include <iostream>
#include <fstream>
#include <string>
#include "Vars.h"
#include "Pdb.h"
using namespace std;

Pdb::Pdb(string file_name) {
    name = file_name;

    string line;
    ifstream f(name);

    // Read file with name "file_name" into "file_lines" instance variable
    if (f) {
        while (getline(f, line)) {
            file_lines.push_back(line);
        }
    }
    else {
        cout << "File not found. Empty file loaded." << endl;
    }
    f.close();
}

Pdb::~Pdb() {
}

string Pdb::get_name() {
    return name;
}

string Pdb::get_AA_seq() {
    return AA_seq;
}

vector<string> Pdb::get_file_lines() {
    return file_lines;
}

void Pdb::load_AA_seq() {
    // This allows future calls to AA_seq-dependent functions, like "print_AA()", to know that the AA_seq has already been loaded
    AA_seq_loaded = true;
    Vars v;
    string residue_seq_num = "";
    AA_seq = "";

    // Read residue codes from "ATOM" lines in .pdb file
    for (string line : file_lines) {
        if (line.find("ATOM") == 0) {
            if (residue_seq_num != line.substr(22, 4)) {
                residue_seq_num = line.substr(22, 4);
                AA_seq += v.AA_codes_3_to_1_map[line.substr(17, 3)];
            }
        }  
    }
}

void Pdb::print_AA_seq() {
    if (!AA_seq_loaded) {
        this->load_AA_seq();
    }
    cout << AA_seq << endl;
    cout << "Length: " << AA_seq.length() << endl;
}

void Pdb::create_fasta(string file_name) {
    if (!AA_seq_loaded) {
        AA_seq_loaded = true;
        this->load_AA_seq();
    }
    ofstream f(file_name);
    // Begin created .fasta file with comment containing the .pdb file's name
    if (f.is_open()) {
        f << ">" << name << "\n" << AA_seq;
    }
    f.close();
}

void Pdb::prune(string mode) {
    // Only implemented modes are "350" and "all"
    // See comments in ProtKit.cpp for details
    if (!(mode == "350" || mode == "all")) {
        cout << "Mode not recognized." << endl;
        return;
    }

    // Get list of indices to be removed by pruning
    vector<int> indices_to_be_removed;
    if (mode == "350") {
        for (unsigned int i = 0; i < file_lines.size(); i++) {
            if (!(file_lines[i].find("ATOM") == 0 ||
                       file_lines[i].find("REMARK 350") == 0)) {
                indices_to_be_removed.push_back(i);
            }
        }
    }
    else if (mode == "all") {
        for (unsigned int i = 0; i < file_lines.size(); i++) {
            if (!(file_lines[i].find("ATOM") == 0)) {
                indices_to_be_removed.push_back(i);
            }
        }
    }

    // After getting "indices_to_be_removed", remove the lines at those original indices
    int lines_removed = 0;
    for (int i : indices_to_be_removed) {
        file_lines.erase(file_lines.begin() + (i - lines_removed++));
    }
}
