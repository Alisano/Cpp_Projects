#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include "geom_cls.hpp"
#include "init_geom.hpp"
#include "Eigen/Core"
#include "Eigen/Dense"
#include <numeric>
#include "Eigen/IterativeLinearSolvers"
#include "solver_field.hpp"

#pragma once
#ifndef TRAJ_H
#define TRAJ_H

void traj(std::vector<double>& pos_part, std::vector<double>& vel0_part, std::vector<double>& time_arr, double dt, std::vector<Geom*>& objects, 
          std::vector<double>& part_share_arr, std::vector<int>& particle_counter, 
          std::vector<double>& energies, std::vector<int>& elements_index_without_computing, 
          std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, Eigen::VectorXd &potentials_array, 
          double &electric_field_x, double &electric_field_y, double &electric_field_z,
          std::vector<std::vector<double>> &result, std::vector<std::vector<double>> &pos_sec_part, std::vector<std::vector<double>> &vel_sec_part);
double random(double min, double max);
void intersect_emission (std::vector<double> vel_xy, std::vector<double> pos_part,
                         std::vector<double> time_arr, double dt,
                         std::vector<Geom*> &objects, std::vector<double> &part_share_arr, 
                         std::vector<int> &particle_counter, std::vector<double> &energies, 
                         std::vector<int> &elements_index_without_computing, 
                         std::vector<std::vector<double>> &pos_sec_part,
                         std::vector<std::vector<double>> &vel_sec_part);
void solver_traj(std::vector<double>& pos_part, std::vector<double>& vel0_part, std::vector<double>& time_arr, double dt, std::vector<Geom*>& objects, 
                 std::vector<double>& part_share_arr, std::vector<int>& particle_counter, 
                 std::vector<double>& energies, std::vector<int>& elements_index_without_computing,
                 std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, Eigen::VectorXd &potentials_array, 
                 double &electric_field_x, double &electric_field_y, double &electric_field_z,
                 std::vector<std::vector<double>> &result, std::vector<std::vector<double>> &pos_sec_part, std::vector<std::vector<double>> &vel_sec_part,
                 std::vector<std::vector<std::vector<double>>> &pos_sec_part_arr,
                 std::vector<std::vector<std::vector<double>>> &vel_sec_part_arr,
                 std::vector<std::vector<std::vector<double>>> &result_arr);
#endif