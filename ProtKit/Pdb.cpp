#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Pdb.h"
using namespace std;

// Constructor for loading from .pdb file
Pdb::Pdb(string file_name) {
    name = file_name;

    // I/O variables
    string line, temp_string;
    stringstream ss;
    ifstream f(name);
    Vars v;
    unsigned int last_residue_number = 0;

    // Variables to serve as arguments from AtomLine constructor
    string atom_name;
    string AA_residue;
    char chain;
    unsigned int residue_number;
    vector<double> coordinates(3);
    double occupancy;
    double b_factor;

    if (f) {
        // Read each line of .pdb file into an AtomLine object that is added to `atom_lines`
        while (getline(f, line)) {
            if (line.find("ATOM") == 0) {
                atom_name = remove_spaces_from_string(line.substr(12, 4));
                AA_residue = line.substr(17, 3);
                chain = line.substr(21)[0];
                residue_number = stoi(line.substr(22, 4));
                coordinates[0] = stod(line.substr(30, 8));
                coordinates[1] = stod(line.substr(38, 8));
                coordinates[2] = stod(line.substr(46, 8));
                occupancy = stod(line.substr(54, 6));
                b_factor = stod(line.substr(60, 6));
                
                atom_lines.push_back(AtomLine(atom_name, AA_residue, chain, residue_number, coordinates, occupancy, b_factor));

                // Translate `AA_residue` from a 3-letter code to a 1-letter code and store it in `AA_seq`
                if (residue_number != last_residue_number) {
                    last_residue_number = residue_number;
                    AA_seq += v.AA_codes_3_to_1_map[AA_residue];
                }
            }
        }
    }
    else {
        // Do this if the file name is not valid
        cout << "File not found. Empty file loaded." << endl;
    }
    f.close();
}

// Writes a .fasta file from the .pdb file
void Pdb::write_fasta(string file_name) {
    ofstream f(file_name);
    // Begin created .fasta file with comment containing the .pdb file's name
    f << ">" << name << "\n" << AA_seq;
    f.close();
}

// Returns vector containing mean coordinates of all atoms in the .pdb file
vector<double> Pdb::get_mean_atom_coords() {
    vector<double> means(3);

    // Iterate through all of atom_coords, summing values into "means" vector
    for (AtomLine i : atom_lines) {
        for (int j = 0; j < 3; j++) {
            means[j] += i.coordinates[j];
        }
    }

    // Divide by number of atoms
    unsigned int num_of_atoms = atom_lines.size();
    for (int i = 0; i < 3; i++) {
        means[i] /= num_of_atoms;
    }

    return means;
}

// Writes a properly-formatted .pdb file from information stored in the instance variable `atom_lines`
void Pdb::write_file(string file_name) {
    ofstream f(file_name);
    string index_string;
    string residue_number_string;

    for (unsigned int i = 0; i < atom_lines.size(); i++) {
        index_string = to_string((i % 99999) + 1);
        residue_number_string = to_string(atom_lines[i].residue_number);

        f << "ATOM  " << string(5 - index_string.length(), ' ')
            << index_string << "  "
            << atom_lines[i].atom_name
            << string(4 - atom_lines[i].atom_name.length(), ' ')
            << atom_lines[i].AA_residue << ' '
            << atom_lines[i].chain << string(4 - residue_number_string.length(), ' ') 
            << atom_lines[i].residue_number << string(5, ' ')
            << convert_double_to_string_of_certain_length(atom_lines[i].coordinates[0], 7) << ' '
            << convert_double_to_string_of_certain_length(atom_lines[i].coordinates[1], 7) << ' '
            << convert_double_to_string_of_certain_length(atom_lines[i].coordinates[2], 7) << "  "
            << convert_double_to_string_of_certain_length(atom_lines[i].occupancy, 4) << ' '
            << convert_double_to_string_of_certain_length(atom_lines[i].b_factor, 5) << string(11, ' ')
            << get_first_alphabet_character_from_string(atom_lines[i].atom_name) << endl;
    }
    f << "TER" << endl;
    f.close();
}

// Appends `atom_lines` and `AA_seq` of argument `Pdb` object to instance variables
void Pdb::append_file(Pdb file) {
    // Make `residue_number` values of appended `AtomLine` objects match the residue number of this object
    unsigned int new_residue_number = 1;
    if (atom_lines.size())
        new_residue_number = atom_lines.back().residue_number + 1;

    // Used to identify change of `residue_number` in `file`
    unsigned int last_residue_number = file.atom_lines[0].residue_number;

    for (AtomLine& al : file.atom_lines) {
        // Increment `new_residue_number` upon reaching next residue
        if (last_residue_number != al.residue_number) {
            last_residue_number = al.residue_number;
            new_residue_number++;
        }

        // Prevent `last_residue_number` from exceeding the 4 characters that can be store in a .pdb file
        if (new_residue_number == 10000)
            new_residue_number = 1;

        // Update `residue_number` of `AtomLine` in `file` before appending
        al.residue_number = new_residue_number;
    }

    // Append
    atom_lines.insert(atom_lines.end(), file.atom_lines.begin(), file.atom_lines.end());
    AA_seq += file.AA_seq;
}

// Gets `atom_lines` with argument `col_vector` added to coordinates of every entry
vector<AtomLine> Pdb::add_vector(vector<double> col_vector) {
    vector<AtomLine> return_vector;

    for (AtomLine i : atom_lines) {
        i.add_vector(col_vector);
        return_vector.push_back(i);
    }

    return return_vector;
}

// Gets `atom_lines` with every entry left-multiplied by argument `sq_matrix`
vector<AtomLine> Pdb::mult_matrix(vector<vector<double>> sq_matrix) {
    vector<AtomLine> return_vector;

    for (AtomLine i : atom_lines) {
        i.mult_matrix(sq_matrix);
        return_vector.push_back(i);
    }

    return return_vector;
}
