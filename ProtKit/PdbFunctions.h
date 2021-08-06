#pragma once
#include "Fncs.h"

double get_rmsd(Pdb* p1, Pdb* p2) {
    std::vector<unsigned int> overlap = fncs::get_longest_common_substring_indices(
        p1->AA_seq,
        p2->AA_seq);

    p1->load_CA_indices();
    p2->load_CA_indices();

    double sum_squared_distance = 0;
    for (unsigned int i = 0; i < overlap[2]; i++)
        sum_squared_distance += fncs::get_distance_squared(
            p1->atom_lines[p1->CA_indices[overlap[0] + i]].get_coordinates(),
            p2->atom_lines[p2->CA_indices[overlap[1] + i]].get_coordinates());

    return pow(sum_squared_distance / overlap[2], 0.5);
}

double get_tm_score(Pdb* p1, Pdb* p2) {
    std::vector<unsigned int> overlap = fncs::get_longest_common_substring_indices(
        p1->AA_seq,
        p2->AA_seq);

    p1->load_CA_indices();
    p2->load_CA_indices();

    unsigned int L_target = p1->AA_seq.length();
    double d0_squared = pow(1.24 * pow(L_target - 15, 1.0 / 3.0) - 1.8, 2);
    double tm_score = 0;
    for (unsigned int i = 0; i < overlap[2]; i++) {
        tm_score += 1 / (1 + (fncs::get_distance_squared(
            p1->atom_lines[p1->CA_indices[overlap[0] + i]].get_coordinates(),
            p2->atom_lines[p2->CA_indices[overlap[1] + i]].get_coordinates())
            / d0_squared));
    }

    return tm_score / L_target;
}