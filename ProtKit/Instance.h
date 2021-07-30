#pragma once
#include <string>
#include <vector>
#include "ProtFile.h"
#include "Pdb.h"
#include "Fasta.h"
#include "Config.h"
using namespace std;

class Instance {
public:
    Instance(string config_file);
    ~Instance();
    void process_input(string line, ifstream& f);

    // Stores all loaded files
    vector<ProtFile*> loaded_files;
    // Stores active index of `loaded_files`
    unsigned int active_index = 0;
    // Stores .pdb file being modified
    Pdb* working_file;
    // Stores user-defined functions
    unordered_map <string, vector<string>> funcs;
    // Stores name and version number
	string version = "ProtKit v0.20";
    // Pointer to `Config` object that stores info from config file at path `config_file`
    Config* config;
    // Loads constants from `Vars` class
    Vars v;
};