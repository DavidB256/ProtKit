#include "Fncs.h"
using namespace std;

double Fncs::get_rmsd(Pdb* p1, Pdb* p2) {
    vector<unsigned int> overlap = get_longest_common_substring_indices(
        p1->get_AA_seq(),
        p2->get_AA_seq());

    p1->load_CA_indices();
    p2->load_CA_indices();

    double sum_squared_distance = 0;
    for (unsigned int i = 0; i < overlap[2]; i++)
        sum_squared_distance += get_distance_squared(
            p1->atom_lines[p1->CA_indices[overlap[0] + i]].coordinates,
            p2->atom_lines[p2->CA_indices[overlap[1] + i]].coordinates);

    return pow(sum_squared_distance / overlap[2], 0.5);
}

double Fncs::get_tm_score(Pdb* p1, Pdb* p2) {
    return 0;
}