#pragma once
#ifndef SOLVER_E_H
#define SOLVER_E_H

#include <vector>
#include <iostream>
#include "Eigen/Core"
#include "Eigen/Dense"
#include <numeric>
#include "Eigen/IterativeLinearSolvers"
#include <cmath>

void arr_for_triple_product(std::vector<double> &pos, std::vector<double> &point1, std::vector<double> &point2, std::vector<double> &point3, Eigen::Matrix3d &array_p);
bool point_tetra(std::vector<double> &pos, std::vector<std::vector<double>> &tetra_coord);
void E_field_coef(std::vector<double> &pos, int coef_num, std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, Eigen::VectorXd &potentials_array, double &electric_field);
void E_field_x(std::vector<double> &pos, std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, Eigen::VectorXd &potentials_array, double &electric_field_x);
void E_field_y(std::vector<double> &pos, std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, Eigen::VectorXd &potentials_array, double &electric_field_y);
void E_field_z(std::vector<double> &pos, std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, Eigen::VectorXd &potentials_array, double &electric_field_z);
double E_field_mod(std::vector<double> &pos, std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, 
                    Eigen::VectorXd &potentials_array, double &electric_field_x,  double &electric_field_y, double &electric_field_z);

#endif