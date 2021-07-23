load a.pdb
mult_matrix 1 0 0 / 0 1 0 / 0 0 1
mult_matrix -0.5 -0.866 0 / 0.866 -0.5 0 / 0 0 1
mult_matrix -0.5 0.866 0 / -0.866 -0.5 0 / 0 0 1
mult_matrix -1 0 0 / 0 -1 0 / 0 0 1
mult_matrix 0.5 0.866 0 / -0.866 0.5 0 / 0 0 1
mult_matrix 0.5 -0.866 0 / 0.866 0.5 0 / 0 0 1
load_working
self
add_vector 35 60.622 0
add_vector -35 60.622 0
add_vector 35 -60.622 0
add_vector -35 -60.622 0
add_vector 70 0 0
add_vector -70 0 0
load_working
self
add_vector 0 0 40
add_vector 0 0 80
add_vector 0 0 120
add_vector 0 0 160
add_vector 0 0 200
add_vector 0 0 240
load_working
self
axis_rotate 0 0 1 / 0 0 0 / -1.57
axis_rotate 0 0 1 / 0 0 0 / 1.57
axis_rotate 0 0 1 / 0 0 0 / 3.14
write_file alot.pdb