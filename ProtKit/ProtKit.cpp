#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <stack>
#include <filesystem>
#include "Vars.h"
#include "Pdb.h"
#include "Config.h"
#include "AtomLine.h"
#include "Fncs.h"
#include "Instance.h"
using namespace std;

int main() {
    // Stores initial working directory; used when resetting
    filesystem::path initial_directory = filesystem::current_path();
    // Stores directory of config file
    string config_file = "config.txt";
    // Create pointer to new `Instance` object for this run
    Instance* in = new Instance(config_file);
    cout << "Type 'help' for a list of commands." << endl << ">> ";

    // To open script files for reading
    ifstream f;
    // To store user input
    string line;
    // Object used for accessing certain large constants
    Vars v;

    while (true) {
        // Get user input from `cin` or loaded script file
        line = Fncs::get_input(f);
        if (line == "q" || line == "quit") {
            // Quit when asked
            break;
        }
        else if (line == "r" || line == "reset") {
            cout << "Instance reset." << endl;
            // Restore initial working directory so that the config file can be found again in the `Instance` constructor
            filesystem::current_path(initial_directory);
            // Calls deconstructor and creates new `Instance` object pointer
            delete in;
            in = new Instance(config_file);
        }
        else {
            // Run all other commands from `process_input` method of `Instance`
            in->process_input(line, f);
        }

        cout << ">> ";
    }

    // Exit sequence
    cout << endl;
    return 0;
}
