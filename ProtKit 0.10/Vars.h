#pragma once
#include <unordered_map>
using namespace std;

class Vars {
public:
	unordered_map <string, string> AA_codes_3_to_1_map = { {"ALA", "A"},{"ARG", "R"},{"ASN", "N"},{"ASP", "D"},{"CYS", "C"},
		{"GLU", "E"},{"GLN", "Q"},{"GLY", "G"},{"HIS", "H"},{"ILE", "I"},
		{"LEU", "L"},{"LYS", "K"},{"MET", "M"},{"PHE", "F"},{"PRO", "P"},
		{"SER", "S"},{"THR", "T"},{"TRP", "W"},{"TYR", "Y"},{"VAL", "V"} };
	unordered_map <string, string> AA_codes_1_to_3_map = { {"A", "ALA"},{"R", "ARG"},{"N", "ASN"},{"D", "ASP"},{"C", "CYS"},
		{"E", "GLU"},{"Q", "GLN"},{"G", "GLY"},{"H", "HIS"},{"I", "ILE"},
		{"L", "LEU"},{"K", "LYS"},{"M", "MET"},{"F", "PHE"},{"P", "PRO"},
		{"S", "SER"},{"T", "THR"},{"W", "TRP"},{"Y", "TYR"},{"V", "VAL"} };
};