#pragma once
#include <fstream>
#include <filesystem>

class Config {
public:
	Config(std::string config_path) {
        std::string line;
        std::ifstream f(config_path);

        if (f) {
            while (std::getline(f, line)) {
                // Set working directory from config file
                if (line.find("wd=") == 0) {
                    // Handle potential error of non-existent working directory path in config file
                    try {
                        std::filesystem::current_path(line.substr(3));
                    }
                    catch (int e) {
                        (void)e;
                        std::cout << "Invalid working directory in config file." << std::endl <<
                            "Assuming default working directory: " << default_working_directory << std::endl;
                        std::filesystem::current_path(default_working_directory);
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
            std::cout << "Config file not found." << std::endl <<
                "Assuming default wording directory: " << default_working_directory << std::endl;
            std::filesystem::current_path(default_working_directory);
        }
        f.close();
	}

    std::string default_working_directory = "C:\\Users\\Public\\";
    std::string default_output_pdb = "output.pdb";
};
