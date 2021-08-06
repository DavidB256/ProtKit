#pragma once
#include <unordered_map>

class Vars {
public:
	std::string help_menu = 
		"quit\n"
		"reset\n"
		"reset_working\n"
		"set_wd\n"
		"script <input file_name>\n"
		"load <input file_name>\n"
		"print_AA\n"
		"print_loaded\n"
		"write_fasta <output file_name>\n"
		"back\n"
		"forward\n"
		"find\n"
		"del_active\n"
		"print_active\n"
		"print_3_letter_AA_seq\n"
		"write_file <output file_name>\n"
		"self\n"
		"add_vector <translation 3-vector>\n"
		"mult_matrix <rotation 3x3 matrix>\n"
		"axis_rotate <axis 3-vector, point 3-vector, angle number>\n"
		"move_to <destination 3-vector>\n"
		"center\n"
		"print_rmsd\n"
		"print_tm_score\n"
		"load_working\n"
		"print_bounds";
	std::unordered_map <std::string, char> AA_codes_3_to_1_map = { {"ALA", 'A'},{"ARG", 'R'},{"ASN", 'N'},{"ASP", 'D'},{"CYS", 'C'},
		{"GLU", 'E'},{"GLN", 'Q'},{"GLY", 'G'},{"HIS", 'H'},{"ILE", 'I'},
		{"LEU", 'L'},{"LYS", 'K'},{"MET", 'M'},{"PHE", 'F'},{"PRO", 'P'},
		{"SER", 'S'},{"THR", 'T'},{"TRP", 'W'},{"TYR", 'Y'},{"VAL", 'V'} };
	std::unordered_map <char, std::string> AA_codes_1_to_3_map = { {'A', "ALA"},{'R', "ARG"},{'N', "ASN"},{'D', "ASP"},{'C', "CYS"},
		{'E', "GLU"},{'Q', "GLN"},{'G', "GLY"},{'H', "HIS"},{'I', "ILE"},
		{'L', "LEU"},{'K', "LYS"},{'M', "MET"},{'F', "PHE"},{'P', "PRO"},
		{'S', "SER"},{'T', "THR"},{'W', "TRP"},{'Y', "TYR"},{'V', "VAL"} };
};