#include <iostream>
#include <fstream>
#include <string>
#include "ProtFile.h"
using namespace std;

ProtFile::ProtFile() {
}

ProtFile::ProtFile(string file_name) {
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

ProtFile::~ProtFile() {
}

string ProtFile::get_name() {
    return name;
}

string ProtFile::get_AA_seq() {
    return AA_seq;
}

vector<string> ProtFile::get_file_lines() {
    return file_lines;
}

void ProtFile::load_AA_seq() {
    cout << "Unable to load amino acid sequence for this file type." << endl;
}

void ProtFile::print_AA_seq() {
    cout << AA_seq << endl;
}

void ProtFile::create_fasta(string file_name) {
    cout << "Unable to create .fasta file for this file type." << endl;
}

void ProtFile::prune(string mode) {
    cout << "Unable to prune information from this file type." << endl;
}

void ProtFile::transform(string mode) {
    cout << "Unable to transform this file type." << endl;
}