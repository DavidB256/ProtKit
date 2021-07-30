#include <iostream>
#include "Instance.h"
using namespace std;

// Instance constructor
Instance::Instance(string config_file) {
    // Print name and version
    cout << version << endl;
    // Read config file into `Config` object pointer `config`
    config = new Config(config_file);
    // Add "empty" `Pdb` object to `working_files_vector`
    working_file = new Pdb(vector<AtomLine>());
}

// Instance destructor
Instance::~Instance() {
    for (ProtFile* i : loaded_files)
        delete i;
    delete working_file;
    delete config;
}

void Instance::process_input(string line, ifstream& f) {
    string command = get_nth_word_from_string(line, 1);

    // Identify command from input and act accordingly
    if (command == "help") {
        cout << v.help_menu << endl;
    }
    else if (command == "s" || command == "script") {
        // Run commands from script
        f.open(get_nth_word_from_string(line, 2));
    }
    else if (command == "load") {
        // Loads a file by name into `loaded_files` and sets `active_index` to its index value
        string file_name = get_nth_word_from_string(line, 2);
        string file_extension = file_name.substr(file_name.find_last_of('.') + 1);
        if (file_extension == "pdb") {
            loaded_files.push_back(new Pdb(file_name));
            cout << "Loaded .pdb file." << endl;
        }
        else if (file_extension == "fasta") {
            loaded_files.push_back(new Fasta(file_name));
            cout << "Loaded .fasta file." << endl;
        }
        else {
            loaded_files.push_back(new ProtFile(file_name));
            cout << "Loaded file with unrecognized type." << endl;
        }

        active_index = loaded_files.size() - 1;
    }
    else if (command == "print_AA") {
        if (get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        loaded_files[active_index]->print_AA_seq();
    }
    else if (command == "list_loaded") {
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
        if (get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        loaded_files[active_index]->write_fasta(get_nth_word_from_string(line, 2));
    }
    else if (command == "back") {
        if (get_whether_loaded_files_is_too_short(loaded_files, 1))
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
        if (get_whether_loaded_files_is_too_short(loaded_files, 1))
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
        string file_name = get_nth_word_from_string(line, 2);
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
        if (get_whether_loaded_files_is_too_short(loaded_files, 1))
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
        if (get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Prints the name of the active file in `loaded_files`, if there is one
        if (loaded_files.size())
            cout << "The active file is " << loaded_files[active_index]->get_name() << endl;
    }
    else if (command == "print_3_letter_AA_seq") {
        if (get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Prints AA sequence of active file with space-separated 3-letter codes
        for (char i : loaded_files[active_index]->get_AA_seq())
            cout << v.AA_codes_1_to_3_map[i] << " ";
        cout << endl;
    }
    else if (command == "write_file") {
        // Writes latest entry in `working_files_vector` to a .pdb file in `working_directory`
        working_file->write_file(get_nth_word_from_string(line, 2));
    }
    else if (command == "self") {
        if (get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Appends copy of active file to latest entry in `working_files_vector`
        // Equivalent to, e.g., "add_vector 0 0 0" or "mult_matrix 1 0 0 / 0 1 0 / 0 0 1
        working_file->append(Pdb(loaded_files[active_index]->get_atom_lines()));
    }
    else if (command == "add_vector") {
        if (get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Get translation vector from input
        vector<double> col_vector(3);
        for (int i = 0; i < 3; i++)
            col_vector[i] = stod(get_nth_word_from_string(line, i + 2));

        // Append translated `AtomLine` objects to `atom_lines` of latest element in `working_files_vector`
        working_file->append(Pdb(loaded_files[active_index]->add_vector(col_vector)));
    }
    else if (command == "mult_matrix") {
        if (get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Get matrix from input
        vector<vector<double>> sq_matrix(3, vector<double>(3));
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                sq_matrix[i][j] = stod(get_nth_word_from_string(line, j + (i * 4) + 2));
            }
        }

        // Append transformed `AtomLine` objects to `atom_lines` of latest element in `working_files_vector`
        working_file->append(Pdb(loaded_files[active_index]->mult_matrix(sq_matrix)));
    }
    else if (command == "axis_rotate") {
        if (get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        // Get variables from input: axis vector, point that vector goes through, and angle of rotation
        vector<double> axis(3);
        vector<double> point(3);
        for (int i = 0; i < 3; i++) {
            axis[i] = stod(get_nth_word_from_string(line, i + 2));
            point[i] = stod(get_nth_word_from_string(line, i + 6));
        }
        double angle = stod(get_nth_word_from_string(line, 10));

        // Append transformed `AtomLine` objects to `atom_lines` of latest element in `working_files_vector`
        working_file->append(Pdb(loaded_files[active_index]->axis_rotate(axis, point, angle)));
    }
    else if (command == "move_to") {
        if (get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        vector<double> mean_coords = loaded_files[active_index]->get_mean_atom_coords();
        // Get destination vector from input and subtract `mean_coords`, current mean location of the protein, from each entry
        vector<double> col_vector(3);
        for (int i = 0; i < 3; i++)
            col_vector[i] = stod(get_nth_word_from_string(line, i + 2)) - mean_coords[i];

        // Append translated `AtomLine` objects to `atom_lines` of latest element in `working_files_vector`
        working_file->append(Pdb(loaded_files[active_index]->add_vector(col_vector)));
    }
    else if (command == "center") {
        // Translates active protein such that its center is at the origin
        // Equivalent to "move_to 0 0 0"
        if (get_whether_loaded_files_is_too_short(loaded_files, 1))
            return;

        vector<double> col_vector = loaded_files[active_index]->get_mean_atom_coords();
        for (double& i : col_vector)
            i *= -1;

        working_file->append(Pdb(loaded_files[active_index]->add_vector(col_vector)));
    }
    else if (command == "load_working") {
        // Load latest entry in `working_files_vector` into `loaded_files`, making it the new file to be acted upon by commands
        loaded_files.push_back(new Pdb(to_string(loaded_files.size()) + "_" +
            config->default_output_pdb, working_file->get_atom_lines()));
        active_index = loaded_files.size() - 1;
    }
    else if (line == "" || line.find("/") == 0) {
        // Ignore blank lines and comments, which are denoted with '/'
    }
    else {
        cout << "Command '" << command << "' not recognized." << endl;
    }
}