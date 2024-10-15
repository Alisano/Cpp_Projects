#pragma once
#ifndef SOLVER_P_H
#define SOLVER_P_H

#include <vector>
#include <iostream>
#include "Eigen/SparseCore"
#include "Eigen/Core"
#include "Eigen/Sparse"
#include "Eigen/Dense"
#include <algorithm>
#include <execution>
#include <map>
#include <thread>
#include <numeric>
#include "geom_cls.hpp"
#include "Eigen/SparseCholesky"
#include "Eigen/IterativeLinearSolvers"
#include <fstream>
#include <algorithm>

void dict_of_nodes_in_tetra(std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, std::vector<std::vector<int>> &adj_list);
void global_matrix_of_stiffness_computator_thread(std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, 
                                                  std::vector<std::vector<int>> &adj_list, Eigen::SparseMatrix<double> &global_matrix_of_stiffness, 
                                                  std::map<int, std::map<int, double>> &cache_gms, int i_begin, int i_end);
void global_matrix_of_stiffness_computator_parallel(std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, 
                                                    std::vector<std::vector<int>> &adj_list, Eigen::SparseMatrix<double> &global_matrix_of_stiffness, const int CPU_COUNT);
void global_matrix_boundary_condition_parallel(Eigen::SparseMatrix<double> &global_matrix_of_stiffness, std::vector<std::vector<double>> &vertex, 
                                               std::vector<Geom*> &objects,  Eigen::VectorXd &potentials_array, const int CPU_COUNT);
void global_matrix_boundary_condition_thread(std::vector<std::vector<double>> &vertex, std::vector<Geom*> &objects,  std::vector<bool> &chng_ind, int i_begin, int i_end);
void solver_potential(std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, std::vector<Geom*> &objects, Eigen::VectorXd &potentials_array) ;

#endif