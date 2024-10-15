#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "src_c/init_geom.hpp"
#include "src_c/solver_potential.hpp"
#include "src_c/solver_field.hpp"
#include "src_c/solver_traj.hpp"
#include <time.h>
#include <random>

std::vector<std::string> splitString(std::string str, char splitter){
    std::vector<std::string> result;
    std::string current = ""; 
    for (int i = 0; i < str.size(); i++){
        if (str[i] == splitter){
            if(current != ""){
                result.push_back(current);
                current = "";
            } 
            continue;
        }
        current += str[i];
    }
    if(current.size() != 0)
        result.push_back(current);
    return result;
}

template <typename T>
std::vector<std::vector<T>> parser_str(std::vector<std::string> &target) 
{
    char delimiter = ',';
    std::vector<std::vector<T>> res;
    std::vector<T> tmp;

    for (auto str : target) 
    {
        for (std::string data: splitString(str, delimiter)) {
             tmp.push_back(static_cast<T> (strtod(data.c_str(), NULL)));
        }

        res.push_back(tmp);
        tmp.clear();
    }

    return res;
}

void parser_txt(std::vector<std::string> &target, std::string name_file) {
    std::ifstream in;
    std::string tmp;
    std::string file_dir = "./src_py/output_py/";
    in.open(file_dir + name_file);

    if (in.is_open())
    {
        while (std::getline(in, tmp)){
            target.push_back(tmp);
        }
    }
    in.close();
}

template <typename T>
void print(std::vector<std::vector<T>>& arr) 
{
    for (auto row : arr) 
    {
        for (auto el : row)
        {
            std::cout << el << std::endl;
        }
    }
}

template<typename T>
void write_file(std::string file_name, T &data)
{
    std::ofstream out;
    std::string file_path = "./src_c/output_c/";
    out.open(file_path + file_name);
    if (out.is_open())
    {
        for (auto el : data)
        {
            out << el << std::endl;
        }
    }
    out.close(); 
}

template<typename T>
std::vector<T> arange(T start, T stop, T step = 1) {
    std::vector<T> values;
    for (T value = start; value < stop; value += step)
        values.push_back(value);
    return values;
}

template<typename T>
void write_traj_file(std::string file_name, T &data)
{
    std::ofstream out;
    std::string file_path = "./src_c/output_c/";
    out.open(file_path + file_name);
    if (out.is_open())
    {
        for (int i = 0; i < data.size(); i++)
        {
            for (auto vec_pos_part : data[i])
            {
                out << vec_pos_part[0] << " " <<  vec_pos_part[1] << " " <<  vec_pos_part[2] << std::endl;
            }
            out << std::endl;
        }
    }
    out.close(); 
}

void traj_runner(std::vector<std::vector<double>> &pos0_part, 
                std::vector<std::vector<std::vector<double>>> &result_arr,
                std::vector<double> &vel0_part , double &dt, std::vector<double> &time_arr, 
                std::vector<double> &part_share_arr, std::vector<int> &particle_counter,  
                std::vector<double> &energies, std::vector<int> &elements_index_without_computing,
                std::vector<Geom*> &objects,  std::vector<std::vector<double>> &vertex, 
                std::vector<std::vector<int>> &tetra, Eigen::VectorXd &potentials, 
                double &electric_field_x, double &electric_field_y, double &electric_field_z) 
{
    std::vector<std::vector<double>> result;
    std::vector<std::vector<double>> pos_sec_part; 
    std::vector<std::vector<double>> vel_sec_part;

    std::vector<std::vector<std::vector<double>>> tmp_result_arr;
    std::vector<std::vector<std::vector<double>>> pos_sec_part_arr;
    std::vector<std::vector<std::vector<double>>> vel_sec_part_arr;

    for (int i = 0; i < pos0_part.size(); i++)
    {
        solver_traj(pos0_part[i], vel0_part, time_arr, dt, objects, 
                    part_share_arr, particle_counter, 
                    energies, elements_index_without_computing, 
                    vertex, tetra, potentials, 
                    electric_field_x, electric_field_y, electric_field_z,
                    result, pos_sec_part, vel_sec_part,
                    pos_sec_part_arr, vel_sec_part_arr, tmp_result_arr);

        result_arr.insert(std::end(result_arr), std::begin(tmp_result_arr), std::end(tmp_result_arr));

        tmp_result_arr.clear();
        vel_sec_part_arr.clear();
        tmp_result_arr.clear();

        result.clear();
        pos_sec_part.clear();
        vel_sec_part.clear();
    }
}

int main(int argc, char* argv[]) 
{
    std::random_device system_rand;
    int seed = system_rand();
    std::srand(seed);

    std::vector<std::string> vertex_str;
    std::vector<std::string> tetra_str;
    std::string name_file_vertex = "vertex.txt";
    std::string name_file_tetra = "tetra.txt";
    std::vector<std::vector<double>> vertex;
    std::vector<std::vector<int>> tetra;

    parser_txt(vertex_str, name_file_vertex);
    parser_txt(tetra_str, name_file_tetra);
    vertex = parser_str<double>(vertex_str);
    tetra = parser_str<int>(tetra_str);

    std::vector<Geom*> objects; 
    objects = init_geom();
 
    Eigen::VectorXd potentials;
    solver_potential(vertex, tetra, objects, potentials);

    double electric_field_x, electric_field_y, electric_field_z;
    std::vector<double> E_field;

    for (std::vector<double> pos : vertex)
    {     
        E_field.push_back(E_field_mod(pos, vertex, tetra, potentials, electric_field_x,  electric_field_y, electric_field_z));
    }

    write_file<Eigen::VectorXd>("potentials.txt", potentials);
    write_file<std::vector<double>>("field.txt", E_field);

    double dt = 2e-13;
    
    std::vector<double> vel0_part = {0.0, 0.0, 0.0};
    std::vector<double> time_arr = arange<double>(0.0, 5e-11, dt);
    std::vector<double> part_share_arr;
    std::vector<int> particle_counter;
    std::vector<double> energies;
    std::vector<int> elements_index_without_computing;

    std::vector<std::vector<std::vector<double>>> result_arr;
    std::vector< std::vector<double>> pos0_part;

    for (int i = 0; i < 100; i++) 
    {
        pos0_part.push_back({1.575e-04, 6.65301247e-05, double(i) * 2e-8});
    }

    traj_runner(pos0_part, result_arr, vel0_part, dt, time_arr, part_share_arr, 
                particle_counter, energies, elements_index_without_computing, 
                objects, vertex, tetra, potentials, electric_field_x, electric_field_y, electric_field_z);

    write_traj_file<std::vector<std::vector<std::vector<double>>>>("trajectories.txt", result_arr);
    return 0;
}