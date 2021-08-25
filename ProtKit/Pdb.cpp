#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Pdb.h"
#include "Fncs.h"
#include "Vars.h"
using namespace std;


// Constructor for loading from .pdb file
Pdb::Pdb(string file_name) {
    name = file_name;

    // I/O variables
    string line, temp_string;
    stringstream ss;
    ifstream f(name);

    // Variables to serve as arguments from AtomLine constructor
    string atom_name;
    string AA_residue;
    char chain;
    unsigned int residue_number;
    vector<double> coordinates(3);
    double occupancy;
    double b_factor;

    if (f) {
        // Read each 'ATOM' line of the .pdb file into an AtomLine object that is added to `atom_lines`
        while (getline(f, line)) {
            if (line.find("ATOM") == 0) {
                atom_name = fncs::remove_spaces_from_string(line.substr(12, 4));
                AA_residue = line.substr(17, 3);
                chain = line.substr(21)[0];
                residue_number = stoi(line.substr(22, 4));
                coordinates[0] = stod(line.substr(30, 8));
                coordinates[1] = stod(line.substr(38, 8));
                coordinates[2] = stod(line.substr(46, 8));
                occupancy = stod(line.substr(54, 6));
                b_factor = stod(line.substr(60, 6));

                atom_lines.push_back(AtomLine(atom_name, AA_residue, chain, residue_number, coordinates, occupancy, b_factor));
            }
        }
    }
    else {
        // Do this if the file name is not valid
        cout << "File not found. Empty file loaded." << endl;
    }
    f.close();

    load_AA_seq();
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
        for (int j = 0; j < 3; j++)
            means[j] += i.coordinates[j];
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
            << fncs::convert_double_to_string_of_certain_length(atom_lines[i].coordinates[0], 7) << ' '
            << fncs::convert_double_to_string_of_certain_length(atom_lines[i].coordinates[1], 7) << ' '
            << fncs::convert_double_to_string_of_certain_length(atom_lines[i].coordinates[2], 7) << "  "
            << fncs::convert_double_to_string_of_certain_length(atom_lines[i].occupancy, 4) << ' '
            << fncs::convert_double_to_string_of_certain_length(atom_lines[i].b_factor, 5) << string(11, ' ')
            << fncs::get_first_alphabet_character_from_string(atom_lines[i].atom_name) << endl;
    }
    f << "TER" << endl;
    f.close();
}

// Appends `atom_lines` and `AA_seq` of argument `Pdb` object to instance variables
void Pdb::append(Pdb appendee) {
    // Set `last_chain` to final chain identifier of last `AtomLine` of file being appended to
    // If `atom_lines` is empty, then set `last_chain` to 64, which is the ASCII character before 'A'=65
    char last_chain;
    if (atom_lines.size())
        last_chain = atom_lines.back().chain;
    else
        last_chain = 64;
    
    // Add `last_chain` to all `i.chain` values while keeping `i.chain` within the interval of the uppercase letters in ASCII code, [65, 90]
    for (AtomLine& i : appendee.atom_lines)
        i.chain = (((i.chain - 65) + (last_chain - 65) + 1) % 26) + 65;

    // Do the appending
    atom_lines.insert(atom_lines.end(), appendee.atom_lines.begin(), appendee.atom_lines.end());
    AA_seq += appendee.AA_seq;
}

// Returns `atom_lines` with argument `col_vector` added to coordinates of every entry
vector<AtomLine> Pdb::add_vector(vector<double> col_vector) {
    vector<AtomLine> return_vector;

    for (AtomLine i : atom_lines) {
        i.add_vector(col_vector);
        return_vector.push_back(i);
    }

    return return_vector;
}

// Returns `atom_lines` with every entry left-multiplied by argument `sq_matrix`
vector<AtomLine> Pdb::mult_matrix(vector<vector<double>> sq_matrix) {
    vector<AtomLine> return_vector;

    for (AtomLine i : atom_lines) {
        i.mult_matrix(sq_matrix);
        return_vector.push_back(i);
    }

    return return_vector;
}

// Returns `atom_lines` with every atom rotated by `angle` radians around a line parallel to `axis` that goes through `point`
vector<AtomLine> Pdb::axis_rotate(vector<double> axis, vector<double> point, double angle) {
    // `neg_point` is needed to translate the atom before performing the rotation, after which `point` is used for reverse translation
    vector<double> neg_point(3);
    for (int i = 0; i < 3; i++)
        neg_point[i] = -1 * point[i];

    // Create rotation matrix from formula found here: https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
    axis = fncs::normalize_vector(axis);
    double ux2 = pow(axis[0], 2);
    double uy2 = pow(axis[1], 2);
    double uz2 = pow(axis[2], 2);
    double c = cos(angle);
    double cc = 1 - c;
    double s = sin(angle);

    vector<vector<double>> sq_matrix =
    { {c + (ux2 * cc), (axis[0] * axis[1] * cc) - (axis[2] * s), (axis[0] * axis[2] * cc) + (axis[1] * s)},
      {(axis[0] * axis[1] * cc) + (axis[2] * s), c + (uy2 * cc), (axis[1] * axis[2] * cc) - (axis[0] * s)},
      {(axis[0] * axis[2] * cc) - (axis[1] * s), (axis[1] * axis[2] * cc) + (axis[0] * s), c + (uz2 * cc)} };

    return Pdb(Pdb(add_vector(neg_point)).mult_matrix(sq_matrix)).add_vector(point);
}

// This is an incomplete rough draft of a structure alignment method
// Nothing calls this method
/*
vector<AtomLine> Pdb::align_by_first_CAs(Pdb alignee) {
    if (AA_seq.length() < 3 || alignee.AA_seq.length() < 3) {
        cout << "Both template and alignee must have at least be three residues long to execute this function." << endl;
        return vector<AtomLine>();
    }

    vector<unsigned int> template_Ca_indices = get_first_n_CA_indices(3);
    vector<unsigned int> alignee_Ca_indices = alignee.get_first_n_CA_indices(3);

    vector<double> diff_between_first_Ca = subtract_vectors(
        atom_lines[template_Ca_indices[0]].coordinates,
        alignee.atom_lines[alignee_Ca_indices[0]].coordinates);

    Pdb align_step1(alignee.add_vector(diff_between_first_Ca));

    const double pi = 3.14159265358979323846;

    vector<double> diff_between_first_two_template_Ca = subtract_vectors(
        atom_lines[template_Ca_indices[1]].coordinates,
        atom_lines[template_Ca_indices[0]].coordinates);
    vector<double> diff_between_first_two_file_Ca = subtract_vectors(
        atom_lines[alignee_Ca_indices[1]].coordinates,
        atom_lines[alignee_Ca_indices[0]].coordinates);
    vector<double> bisector = add_vectors(
        normalize_vector(diff_between_first_two_file_Ca),
        normalize_vector(diff_between_first_two_template_Ca));
    Pdb align_step2(align_step1.axis_rotate(bisector, atom_lines[template_Ca_indices[0]].coordinates, pi));

    return align_step2.atom_lines;
}
*/

// Sets `AA_seq` by loading it from `atom_lines`
// Called from constructor methods
void Pdb::load_AA_seq() {
    string seq = "";
    Vars v;
    unsigned int last_residue_number = 0;

    for (AtomLine al : atom_lines) {
        // Check whether the next `AtomLine` in `atom_lines` is the next residue
        if (al.residue_number != last_residue_number) {
            last_residue_number = al.residue_number;
            seq += v.AA_codes_3_to_1_map[al.AA_residue];
        }
    }

    AA_seq = seq;
}

// Populates `CA_indices` with the indices in `atom_lines` of all alpha carbon atoms
void Pdb::load_CA_indices() {
    // Instantly return if `CA_indices` has already been loaded
    if (CA_indices.size())
        return;

    for (unsigned int i = 0; i < atom_lines.size(); i++) {
        if (atom_lines[i].atom_name == "CA")
            CA_indices.push_back(i);
    }
}