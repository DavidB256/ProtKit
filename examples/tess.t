load a.pdb
mult_matrix 1 0 0 / 0 1 0 / 0 0 1
mult_matrix -0.5 -0.866 0 / 0.866 -0.5 0 / 0 0 1
mult_matrix -0.5 0.866 0 / -0.866 -0.5 0 / 0 0 1
mult_matrix -1 0 0 / 0 -1 0 / 0 0 1
mult_matrix 0.5 0.866 0 / -0.866 0.5 0 / 0 0 1
mult_matrix 0.5 -0.866 0 / 0.866 0.5 0 / 0 0 1
write_to_load
add_vector 35 60.622 0
add_vector -35 60.622 0
add_vector 35 -60.622 0
add_vector -35 -60.622 0
add_vector 70 0 0
add_vector -70 0 0
write_file tesselation.pdb