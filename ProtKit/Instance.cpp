#include <iostream>
#include <unordered_map>
#include <fstream>
#include "Instance.h"
#include "Fncs.h"
#include "PdbFunctions.h"
using namespace std;

// Instance constructor
Instance::Instance(string config_file) {
    // Print name and version
    cout << version << endl;
    // Read config file into `Config` object pointer `config`
    cnfg = new Config(config_file);
    // Add "empty" `Pdb` object to `working_files_vector`
    working_file = new Pdb(vector<AtomLine>());
}

// Instance destructor
Instance::~Instance() {
    for (Pdb* i : loaded_files)
        delete i;
    delete working_file;
    delete cnfg;
}

void Instance::process_input(string line, ifstream& f) {
    string command = fncs::get_nth_word_from_string(line, 1);

    // Identify command from input and act accordingly
    if (command == "help") {
        // Print help menu
        cout << v.help_menu << endl;
    }
    else if (line == "reset_working" || line == "rw") {
        // Delete and reinitialize `working_file`
        delete working_file;
        working_file = new Pdb(vector<AtomLine>());
        cout << "Working file reset." << endl;
    }
    else if (command == "set_wd") {
        // Change the working directory, with error handling for invalid path variables
        try {
            filesystem::current_path(fncs::get_nth_word_from_string(line, 2));
        }
        catch (int e) {
            (void)e;
            cout << "Invalid path input. The working directory has not been changed." << endl;
        }
    }
    else if (command == "s" || command == "script") {
        // Run commands from script
        f.open(fncs::get_nth_word_from_string(line, 2));
    }
    else if (command == "load") {
        // Loads a file by name into `loaded_files` and sets `active_index` to its index value
        string file_name = fncs::get_nth_word_from_string(line, 2);
        if (file_name.length() < 4 || file_name.substr(file_name.length() - 4, 4) != ".pdb")
            file_name += ".pdb";
        
        ifstream f(file_name);
        if (f) {
            loaded_files.push_back(new Pdb(file_name));
            cout << "Loaded " << file_name << "." << endl;

            active_index = loaded_files.size() - 1;
        }
        else {
            cout << "Error: File with name " << file_name << " not found." << endl;
        }
        f.close();
    }
    else if (command == "print_AA") {
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        string seq = loaded_files[active_index]->get_AA_seq();
        cout << seq << endl << "Length: " << seq.length() << endl;
    }
    else if (command == "print_loaded") {
        // Lists all loaded files in `loaded_files` in order of loading
        for (unsigned int i = 0; i < loaded_files.size(); i++) {
            cout << loaded_files[i]->get_name();
            if (i == active_index) {
                cout << "\t\t<-- active file";
            }
            cout << endl;
        }
    }
    else if (command == "write_fasta") {
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        loaded_files[active_index]->write_fasta(fncs::get_nth_word_from_string(line, 2));
    }
    else if (command == "back") {
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Decrements `active_index` to point to previously-loaded file
        if (active_index != 0) {
            active_index--;
            cout << loaded_files[active_index]->get_name() << " is now active." << endl;
        }
        else {
            cout << "There is no prior file to load." << endl;
        }
    }
    else if (command == "forward") {
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Increments `active_index` to point to next-loaded file
        if (active_index != loaded_files.size() - 1) {
            active_index++;
            cout << "Loaded " << loaded_files[active_index]->get_name() << endl;
        }
        else {
            cout << loaded_files[active_index]->get_name() << " is now active." << endl;
        }
    }
    else if (command == "find") {
        // Sets active_index to point to file in `loaded_files` by name
        string file_name = fncs::get_nth_word_from_string(line, 2);
        for (unsigned int i = 0; i < loaded_files.size(); i++) {
            if (loaded_files[i]->get_name() == file_name) {
                active_index = i;
                cout << file_name << " is now active." << endl;
                break;
            }
        }
        cout << "File not found." << endl;
    }
    else if (command == "del_active") {
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Deletes active file from `loaded_files`
        if (loaded_files.size()) {
            cout << "Deleted " << loaded_files[active_index]->get_name() << endl;
            loaded_files.erase(loaded_files.begin() + active_index);
            if (active_index == loaded_files.size())
                active_index--;
        }
    }
    else if (command == "print_active") {
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Prints the name of the active file in `loaded_files`, if there is one
        if (loaded_files.size())
            cout << "The active file is " << loaded_files[active_index]->get_name() << endl;
    }
    else if (command == "print_3_letter_AA_seq") {
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Prints AA sequence of active file with space-separated 3-letter codes
        for (char i : loaded_files[active_index]->get_AA_seq())
            cout << v.AA_codes_1_to_3_map[i] << " ";
        cout << endl;
    }
    else if (command == "write_file") {
        // Writes latest entry in `working_files_vector` to a .pdb file in `working_directory`
        working_file->write_file(fncs::get_nth_word_from_string(line, 2));
    }
    else if (command == "self") {
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Appends copy of active file to latest entry in `working_files_vector`
        // Equivalent to, e.g., "add_vector 0 0 0" or "mult_matrix 1 0 0 / 0 1 0 / 0 0 1
        working_file->append(Pdb(loaded_files[active_index]->get_atom_lines()));
    }
    else if (command == "add_vector") {
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Get translation vector from input
        vector<double> col_vector(3);
        for (int i = 0; i < 3; i++)
            col_vector[i] = stod(fncs::get_nth_word_from_string(line, i + 2));

        // Append translated `AtomLine` objects to `atom_lines` of latest element in `working_files_vector`
        working_file->append(Pdb(loaded_files[active_index]->add_vector(col_vector)));
    }
    else if (command == "mult_matrix") {
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Get matrix from input
        vector<vector<double>> sq_matrix(3, vector<double>(3));
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                sq_matrix[i][j] = stod(fncs::get_nth_word_from_string(line, j + (i * 4) + 2));
            }
        }

        // Append transformed `AtomLine` objects to `atom_lines` of latest element in `working_files_vector`
        working_file->append(Pdb(loaded_files[active_index]->mult_matrix(sq_matrix)));
    }
    else if (command == "axis_rotate") {
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Get variables from input: axis vector, point that vector goes through, and angle of rotation
        vector<double> axis(3);
        vector<double> point(3);
        for (int i = 0; i < 3; i++) {
            axis[i] = stod(fncs::get_nth_word_from_string(line, i + 2));
            point[i] = stod(fncs::get_nth_word_from_string(line, i + 6));
        }
        double angle = stod(fncs::get_nth_word_from_string(line, 10));

        // Append transformed `AtomLine` objects to `atom_lines` of latest element in `working_files_vector`
        working_file->append(Pdb(loaded_files[active_index]->axis_rotate(axis, point, angle)));
    }
    else if (command == "move_to") {
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        vector<double> mean_coords = loaded_files[active_index]->get_mean_atom_coords();
        // Get destination vector from input and subtract `mean_coords`, current mean location of the protein, from each entry
        vector<double> col_vector(3);
        for (int i = 0; i < 3; i++)
            col_vector[i] = stod(fncs::get_nth_word_from_string(line, i + 2)) - mean_coords[i];

        // Append translated `AtomLine` objects to `atom_lines` of latest element in `working_files_vector`
        working_file->append(Pdb(loaded_files[active_index]->add_vector(col_vector)));
    }
    else if (command == "center") {
        // Translates active protein such that its center is at the origin
        // Equivalent to "move_to 0 0 0"
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        vector<double> col_vector = loaded_files[active_index]->get_mean_atom_coords();
        for (double& i : col_vector)
            i *= -1;

        working_file->append(Pdb(loaded_files[active_index]->add_vector(col_vector)));
    }
    else if (command == "print_rmsd") {
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 2))
            return;

        cout << get_rmsd(loaded_files[active_index - 1], loaded_files[active_index]) << endl;
    }
    else if (command == "print_tm_score") {
        if (fncs::get_whether_loaded_files_is_too_short(loaded_files, 2))
            return;

        cout << get_tm_score(loaded_files[active_index - 1], loaded_files[active_index]) << endl;
    }
    else if (command == "print_bounds") {
        // Prints points of diagonally-opposite corners of minimal rectangular prism surrounding protein
        vector<double> min_point = loaded_files[active_index]->get_atom_lines()[0].get_coordinates();
        vector<double> max_point = min_point;

        for (AtomLine al : loaded_files[active_index]->get_atom_lines()) {
            for (int i = 0; i < 3; i++) {
                if (al.get_coordinates()[i] < min_point[i])
                    min_point[i] = al.get_coordinates()[i];
                else if (al.get_coordinates()[i] > max_point[i])
                    max_point[i] = al.get_coordinates()[i];
            }
        }

        cout << "Minimum point: " << min_point[0] << "\t" << min_point[1] << "\t" << min_point[2] << endl <<
            "Maximum point: " << max_point[0] << "\t" << max_point[1] << "\t" << max_point[2] << endl;
    }
    else if (command == "load_working") {
        // Load latest entry in `working_files_vector` into `loaded_files`, making it the new file to be acted upon by commands
        loaded_files.push_back(new Pdb(to_string(loaded_files.size()) + "_" +
            cnfg->default_output_pdb, working_file->get_atom_lines()));
        active_index = loaded_files.size() - 1;
    }
    else if (line == "" || line.find("/") == 0) {
        // Ignore blank lines and comments, which are denoted with '/'
    }
    else {
        cout << "Error: Command '" << command << "' not recognized." << endl;
    }
}