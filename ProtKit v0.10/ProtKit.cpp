#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include "Vars.h"
#include "ProtFile.h"
#include "Pdb.h"
#include "Fasta.h"
using namespace std;

// Forward declarations for functions called in input loop
void help();
void load(string file_name);
void list_loaded();
void create_fasta();
void back();
void forward();
void find();
void del();
void active();
void print_3_letter_AA_seq();
void prune();
void write(vector<string> lines, string file_name);
void transform(int starting_line, string scipt_name);

// Forward declarations for other functions
string modify_atom_line_coords(float coords[3], string atom_line);

// Declare global variables
string line; // To store user input
Vars v;
vector<ProtFile*> file_vector; // To store all loaded files
int active_index = 0; // To store active index of file_vector

int main() {
    // Print name and version
    cout << "ProtKit 0.10" << endl;

    // Set default working directory path for if one cannot  be read from config.txt
    string default_wd = "C:\\Users\\David\\Desktop\\ProtKit workspace";
    // Set path and name of config file
    string config_path = "config.txt";

    // Read config file
    ifstream f(config_path);
    if (f) {
        while (getline(f, line)) {
            // Set working directory from config file
            if (line.find("wd=") == 0) {
                // Handle potential error of non-existent working directory path in config file
                try {
                    filesystem::current_path(line.substr(3));
                }
                catch (int e) {
                    (void)e;
                    cout << "Invalid working directory in config.txt." << endl <<
                        "Assuming default working directory: " << default_wd << endl;
                    filesystem::current_path(default_wd);
                }
            }
        }
    }
    // Actions taken if the config file is not found
    else {
        cout << "config.txt not found." << endl <<
            "Assuming default wording directory: " << default_wd << endl;
        filesystem::current_path(default_wd);
    }
    f.close();

    cout << "Type 'help' for a list of commands." << endl;

    cout << ">> ";
    // Input loop iterates until user inputs "q" or "quit"
    while (getline(cin, line)) {
        if (line == "help") {
            help();
        }
        else if (line == "q" || line == "quit") {
            break;
        }
        else if (line == "load") {
            // "temp_bool" is used to allow uses to cancel loading by entering "q" or "quit"
            bool temp_bool = true;
            do {
                cout << "Input valid file name: ";
                getline(cin, line);
                if (line == "q" || line == "quit") {
                    temp_bool = false;
                    break;
                }
            } while (line.find('.') == -1);
            if (temp_bool) {
                load(line);
            }
        }
        else if (line == "load_AA") {
            file_vector[active_index]->load_AA_seq();
        }
        else if (line == "print_AA") {
            file_vector[active_index]->print_AA_seq();
        }
        else if (line == "list_loaded") {
            list_loaded();
        }
        else if (line == "create_fasta") {
            create_fasta();
        }
        else if (line == "back") {
            back();
        }
        else if (line == "forward") {
            forward();
        }
        else if (line == "find") {
            find();
        }
        else if (line == "del") {
            del();
            cout << "Deleted " << file_vector[active_index]->get_name() << endl;
        }
        else if (line == "active") {
            active();
        }
        else if (line == "print_3_letter_AA_seq") {
            print_3_letter_AA_seq();
        }
        else if (line == "prune") {
            prune();
        }
        else if (line == "write") {
            cout << "Input file name: ";
            getline(cin, line);
            if (!(line == "q" || line == "quit")) {
                write(file_vector[active_index]->get_file_lines(), line);
            }
        }
        else if (line == "transform") {
            cout << "Input script name: ";
            getline(cin, line);
            transform(0, line);
        }
        else {
            cout << "Command not recognized." << endl;
        }
        cout << ">> ";
    }

    // Prevent memory corruption bugs by calling delete on all pointers
    for (ProtFile* i : file_vector) {
        delete i;
    }

    cout << endl;
    return 0;
}

// Prints help menu
void help() {
    cout << "help" << endl;
    cout << "quit" << endl;
    cout << "load" << endl;
    cout << "load_AA" << endl;
    cout << "print_AA" << endl;
    cout << "list_loaded" << endl;
    cout << "create_fasta" << endl;
    cout << "back" << endl;
    cout << "forward" << endl;
    cout << "find" << endl;
    cout << "del" << endl;
    cout << "active" << endl;
    cout << "print_3_letter_AA_seq" << endl;
    cout << "prune" << endl;
    cout << "write" << endl;
    cout << "transform" << endl;
}
// Loads a file by name into "file_vector"
void load(string file_name) {
    string file_extension = file_name.substr(file_name.find_last_of('.') + 1);

    if (file_extension == "pdb") {
        file_vector.push_back(new Pdb(file_name));
    }
    else if (file_extension == "fasta") {
        file_vector.push_back(new Fasta(file_name));
    }
    else {
        file_vector.push_back(new ProtFile(file_name));
    }
    active_index = file_vector.size() - 1;
}
// Lists all loaded files in "file_vector" in order of loading
void list_loaded() {
    for (unsigned int i = 0; i < file_vector.size(); i++) {
        cout << file_vector[i]->get_name();
        if (i == active_index) {
            cout << "\t\t<-- active file";
        }
        cout << endl;
    }
}
// Creates .fasta file from file's AA sequence
// Only implemented for .pdb file type
void create_fasta() {
    cout << "Input file name: ";
    getline(cin, line);
    if (line.find('.') == -1) {
        line += ".fasta";
    }
    file_vector[active_index]->create_fasta(line);
}
// Decrements active_index to point to previously-loaded file
void back() {
    if (active_index != 0) {
        active_index--;
        cout << "Loaded " << file_vector[active_index]->get_name() << endl;
    }
    else {
        cout << "There is no prior file to load." << endl;
    }
}
// Increments active_index to point to next-loaded file
void forward() {
    if (active_index != file_vector.size() - 1) {
        active_index++;
        cout << "Loaded " << file_vector[active_index]->get_name() << endl;
    }
    else {
        cout << "There is no next file to load." << endl;
    }
}
// Sets active_index to point to file in "file_vector" by name
void find() {
    while (true) {
        cout << "Input loaded file name: ";
        getline(cin, line);
        if (line == "q" || line == "quit") {
            return;
        }
        for (unsigned int i = 0; i < file_vector.size(); i++) {
            if (file_vector[i]->get_name() == line) {
                active_index = i;
                cout << "Loaded " << line << endl;
                return;
            }
        }
    }
}
// Deletes active file from file_vector
void del() {
    if (file_vector.size() == 0) {
        cout << "There is no loaded file to delete." << endl;
    }
    else {
        file_vector.erase(file_vector.begin() + active_index);
        if (active_index == file_vector.size()) {
            active_index--;
        }
    }
}
// Prints the name of the active file in "file_vector"
void active() {
    if (file_vector.size() == 0) {
        cout << "There are no loaded files." << endl;
    }
    else {
        cout << "The active file is " << file_vector[active_index]->get_name() << endl;
    }
}
// Prints AA sequence of active file with space-separated 3-letter codes
void print_3_letter_AA_seq() {
    string AA_seq = file_vector[active_index]->get_AA_seq();
    if (AA_seq == "") {
        file_vector[active_index]->load_AA_seq();
    }
    for (char i : AA_seq) {
        line = i;
        cout << v.AA_codes_1_to_3_map[line] << " ";
    }
    cout << endl;
}
// Removes unnecessary lines from file_lines vector of active file
// Only implemented for .pdb file type
// Mode "all" removes all lines except for atom coordinates
// Mode "350" removes all lines except for atom coordinates and "REMARK 350" lines
void prune() {
    cout << "Input pruning mode: ";
    getline(cin, line);
    file_vector[active_index]->prune(line);
}
// Writes a given vector of lines into a file with a given name
void write(vector<string> lines, string file_name) {
    ofstream f(file_name);
    for (string i : lines) {
        f << i << endl;
    }
    f.close();
}
// Performs .pdb file transformations encoded in a given script file
void transform(int starting_line, string script_name) {
    stringstream ss;
    float coords[3], col_vector[3], sq_matrix[3][3], tempx, tempy;
    string temp_string;
    vector<string> lines;
    string output_file = "transform.pdb";
    int line_number = 0;

    // Fully prune active file's "file_lines" vector
    file_vector[active_index]->prune("all");

    // Load script
    ifstream f(script_name);
    // Skip ahead to "starting_line", which is a nonzero argument in recursive cases
    for (int i = 0; i < starting_line; i++) {
        getline(f, line);
    }

    // Loop through lines of script
    while (getline(f, line)) {
        line_number++;
        if (line.find("matrix") == 0) {
            line_number += 3;
            // Load matrix from script into "matrix" array
            for (int i = 0; i < 3; i++) {
                getline(f, line);
                ss.clear();
                ss << line;
                for (int j = 0; j < 3; j++) {
                    ss >> temp_string;
                    sq_matrix[i][j] = stof(temp_string);
                }
            }
            // Iterate through "lines" to perform matrix transformation
            for (string i : file_vector[active_index]->get_file_lines()) {
                // Calculate coordinate transformation
                ss.clear();
                ss << i.substr(30, 24);
                for (int j = 0; j < 3; j++) {
                    ss >> temp_string;
                    coords[j] = stof(temp_string);
                }
                // Right-multiply matrix by vector
                tempx = coords[0];
                tempy = coords[1];
                coords[0] = (sq_matrix[0][0] * coords[0]) + (sq_matrix[0][1] * coords[1]) + (sq_matrix[0][2] * coords[2]);
                coords[1] = (sq_matrix[1][0] * tempx) + (sq_matrix[1][1] * coords[1]) + (sq_matrix[1][2] * coords[2]);
                coords[2] = (sq_matrix[2][0] * tempx) + (sq_matrix[2][1] * tempy) + (sq_matrix[2][2] * coords[2]);

                // Write transformed coordinates into "file_lines"
                lines.push_back(modify_atom_line_coords(coords, i));
            }
        }
        else if (line == "vector") {
            line_number++;

            // Load vector from script into "col_vector" array
            ss.clear();
            getline(f, line);
            ss << line;
            for (int i = 0; i < 3; i++) {
                ss >> temp_string;
                col_vector[i] = stof(temp_string);
            }

            // Iterate through "lines" to perform vector addition
            for (string i : file_vector[active_index]->get_file_lines()) {
                // Calculate coordinate transformation
                ss.clear();
                ss << i.substr(30, 24);
                for (int j = 0; j < 3; j++) {
                    ss >> temp_string;
                    coords[j] = stof(temp_string) + col_vector[j];
                }

                // Write transformed coordinates into "file_lines"
                lines.push_back(modify_atom_line_coords(coords, i));
            }
        }
        else if (line == "recurse") {
            write(lines, output_file);
            load(output_file);
            transform(line_number, script_name);
            del();
            break;
        }
        else if (line.find("op=") == 0) {
            output_file = line.substr(3, line.size()-3);
        }
        else if (line == "self") {
            for (string i : file_vector[active_index]->get_file_lines()) {
                lines.push_back(i);
            }
        }
        else if (line == "stop") {
            break;
        }
        else if (line.find("/") == 0) {
            continue;
        }
        else {
            cout << "Line '" << line << "' not understood by interpreter." << endl;
        }
    }
    f.close();
    
    // Write to final output file
    write(lines, output_file);
}

string modify_atom_line_coords(float coords[3], string atom_line) {
    string temp_string;

    line = atom_line.substr(0, 30);
    for (int i = 0; i < 3; i++) {
        temp_string = " " + to_string(coords[i]);
        if (temp_string.length() > 8) {
            line += temp_string.substr(0, 8);
        }
        else {
            for (int k = temp_string.length(); k < 8; k++) {
                line += " ";
            }
            line += temp_string;
        }
    }
    line += atom_line.substr(54);
    return line;
}