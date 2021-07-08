#include <iostream>
#include <fstream>
#include <string>
#include "Vars.h"
#include "Fasta.h"
using namespace std;

Fasta::Fasta(string file_name) {
    name = file_name;
    string line;
    ifstream f(name);

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

Fasta::~Fasta() {
}

string Fasta::get_name() {
    return name;
}

string Fasta::get_AA_seq() {
    return AA_seq;
}

vector<string> Fasta::get_file_lines() {
    return file_lines;
}

void Fasta::load_AA_seq() {
    AA_seq_loaded = true;
    AA_seq = "";

    for (string line : file_lines) {
        if (!(line[0] == ';' || line[0] == '>')) {
            AA_seq += line;
        }
    }
}

void Fasta::print_AA_seq() {
    if (!AA_seq_loaded) {
        this->load_AA_seq();
    }
    cout << AA_seq << endl;
    cout << "Length: " << AA_seq.length() << endl;
}