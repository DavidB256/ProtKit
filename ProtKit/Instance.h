#pragma once
#include <string>
#include <vector>
#include "Pdb.h"
#include "Config.h"
#include "Vars.h"

class Instance {
public:
    Instance(std::string config_file);
    ~Instance();
    void process_input(std::string line, std::ifstream& f);

    // Stores all loaded files
    std::vector<Pdb*> loaded_files;
    // Stores active index of `loaded_files`
    unsigned int active_index = 0;
    // Stores .pdb file being modified
    Pdb* working_file;
    // Stores user-defined functions
    std::unordered_map <std::string, std::vector<std::string>> funcs;
    // Stores name and version number
	const std::string version = "ProtKit v1.0";
    // Pointer to `Config` object that stores info from config file at path `config_file`
    Config* cnfg;
    // Loads constants from `Vars` class
    Vars v;
};