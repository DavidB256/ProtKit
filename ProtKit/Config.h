#pragma once
#include <fstream>
#include <filesystem>
using namespace std;

class Config {
public:
	Config(string config_path) {
        string line;
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
                        cout << "Invalid working directory for config file." << endl <<
                            "Assuming default working directory: " << working_directory << endl;
                        filesystem::current_path(working_directory);
                    }
                }
                // Set name of default output file of write functions for .pdb files
                else if (line.find("op=") == 0) {
                    default_output_pdb = line.substr(3);
                }
            }
        }
        // Default actions taken if the config file is not found
        else {
            cout << "Config file not found." << endl <<
                "Assuming default wording directory: " << working_directory << endl;
            filesystem::current_path(working_directory);
        }
        f.close();
	}

    string working_directory = "C:\\Users\\Public\\";
    string default_output_pdb = "output.pdb";
};
