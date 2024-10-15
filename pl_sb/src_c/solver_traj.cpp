#include "solver_traj.hpp"

void traj(std::vector<double>& pos_part, std::vector<double>& vel0_part, std::vector<double>& time_arr, double dt, std::vector<Geom*>& objects, 
          std::vector<double>& part_share_arr, std::vector<int>& particle_counter, 
          std::vector<double>& energies, std::vector<int>& elements_index_without_computing, 
          std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, Eigen::VectorXd &potentials_array, 
          double &electric_field_x, double &electric_field_y, double &electric_field_z,
          std::vector<std::vector<double>> &result, std::vector<std::vector<double>> &pos_sec_part, std::vector<std::vector<double>> &vel_sec_part) 
{
    double q = -1.6e-19;
    double m_e = 1.0e-30;

    Geom* obj;
    bool flag_for_elements_index_without_computing;
    std::vector<double> vel_xy;
    // double part_energy;

    double old_vel_x = vel0_part[0];
    double old_vel_y = vel0_part[1];
    double old_vel_z = vel0_part[2];

    std::vector<double> x_array;
    std::vector<double> y_array;
    std::vector<double> z_array;

    for (auto point : vertex)
    {
        x_array.push_back(point[0]);
        y_array.push_back(point[1]);
        z_array.push_back(point[2]);
    }

    for (double t : time_arr) 
    {
        if ((*std::min_element(x_array.begin(), x_array.end()) <= pos_part[0] && pos_part[0] <= *std::max_element(x_array.begin(), x_array.end())) &&
            (*std::min_element(y_array.begin(), y_array.end()) <= pos_part[1] && pos_part[1] <= *std::max_element(y_array.begin(), y_array.end())) &&
            (*std::min_element(z_array.begin(), z_array.end()) <= pos_part[2] && pos_part[2] <= *std::max_element(z_array.begin(), z_array.end()))) 
        {
            result.push_back(std::vector<double>({pos_part[0], pos_part[1], pos_part[2]}));
            flag_for_elements_index_without_computing = false;

            for (int index_obj = 0; index_obj < objects.size(); ++index_obj) 
            {
                obj = objects[index_obj];

                for (int index_of_array : elements_index_without_computing) 
                {
                    if (static_cast<int>(index_of_array) == static_cast<int>(index_obj)) 
                    {
                        flag_for_elements_index_without_computing = true;
                        break;
                    }
                }

                if (flag_for_elements_index_without_computing) 
                {
                    flag_for_elements_index_without_computing = false;
                    continue;
                }
                
                if (obj->intersect_geom_xy(pos_part)) {
                    // if (result.size()) {
                    //     // result.push_back(pos_part[0]); // дополнить
                    //     // result.push_back(pos_part[1]); // дополнить
                    //     // result.push_back(pos_part[2]); // дополнить
                    // }

                    // if (obj->energy_dist) {
                    //     part_energy = abs(m_e * (old_vel_x * old_vel_x + old_vel_y * old_vel_y + old_vel_z * old_vel_z) / (2 * q));
                    //     energies.push_back(part_energy);
                    // }

                    // if (obj->endobj) {
                    // //    result.push_back(-1.0);
                    //     for (size_t i = 0; i < result.size(); ++i)
                    //     {
                    //         if (i > 0){
                    //             part_share_arr.push_back(result[i]);
                    //         }
                    //     }
                    // }

                    if (obj->is_emissive)
                    {
                        vel_xy = {old_vel_x, old_vel_y, old_vel_z};
                        intersect_emission(vel_xy, pos_part,
                                           time_arr, dt,
                                           objects, part_share_arr, 
                                           particle_counter, energies, 
                                           elements_index_without_computing, 
                                           pos_sec_part, vel_sec_part);
                        return;
                    }
                    
                    // result.push_back(-1.0)
                    // for (size_t i = 0; i < result.size(); ++i)
                    // {
                    //     if (i > 0)
                    //     {
                    //         part_share_arr.push_back(result[i]);
                    //     }
                    // }
                    // return;
                }
            }

            E_field_x(pos_part, vertex, tetra, potentials_array, electric_field_x);
            E_field_y(pos_part, vertex, tetra, potentials_array, electric_field_y);
            E_field_z(pos_part, vertex, tetra, potentials_array, electric_field_z);

            old_vel_x += q * electric_field_x * dt / m_e;
            old_vel_y += q * electric_field_y * dt / m_e;
            old_vel_z += q * electric_field_z * dt / m_e;

            pos_part[0] += old_vel_x * dt;
            pos_part[1] += old_vel_y * dt;
            pos_part[2] += old_vel_z * dt;
        }
        else 
        {
            break;
        }
        // result.push_back(-1.0);
        // for (size_t i = 0; i < result.size(); ++i)
        // {
        //     if (i > 0)
        //     {
        //         part_share_arr.push_back(result[i]);
        //     }
        // }
        // return;
    }
}

double random(double min, double max)
{
    return (double)(rand())/RAND_MAX*(max - min) + min;
}

void intersect_emission (std::vector<double> vel_xy, std::vector<double> pos_part,
                         std::vector<double> time_arr, double dt,
                         std::vector<Geom*> &objects, std::vector<double> &part_share_arr, 
                         std::vector<int> &particle_counter, std::vector<double> &energies, 
                         std::vector<int> &elements_index_without_computing, 
                         std::vector<std::vector<double>> &pos_sec_part,
                         std::vector<std::vector<double>> &vel_sec_part) 
{
    std::vector<double> tmp_array;
    double m_e = 1.0e-30;
    double q = -1.6e-19;
    double energy = m_e * (pow(vel_xy[0], 2.0) + pow(vel_xy[1], 2.0)) / 2.0;
    double pair_formation_energy = 20;
    double part_energy = abs(energy / q);
    double n_sec = part_energy / pair_formation_energy;
    // if (n_sec > 1.0) {n_sec = 2.0;}
    double vel_sec = 5.5e5;
    double l_new_pos = 1e-8;
    double pi = 3.1415926;
    double theta;
    double phi;
    bool flag = false;
    std::vector<double> new_pos;
    for (int i = 0 ; i < n_sec; i++)
    {
        flag = false;
        theta = random(0.0, pi);
        phi = random(0.0, 2.0 * pi);
        new_pos = {pos_part[0] + l_new_pos * std::sin(theta) * std::cos(phi),
                   pos_part[1] + l_new_pos * std::sin(theta) * std::sin(phi), 
                   pos_part[2] + l_new_pos * std::cos(theta)};

        // for (auto obj : objects)
        // {
        //     if ((obj->is_emissive) && (obj->intersect_geom_xy(new_pos)))
        //     {
        //         flag = true;
        //         i--;
        //         break;
        //     }
        // }

        if (!flag) 
        {
            pos_sec_part.push_back(new_pos);
            vel_sec_part.push_back(std::vector<double>({vel_sec * std::sin(theta) * std::cos(phi), 
                                   vel_sec * std::sin(theta) * std::sin(phi), 
                                   vel_sec * std::cos(theta)}));
        }
    }
}

void solver_traj(std::vector<double>& pos_part, std::vector<double>& vel0_part, std::vector<double>& time_arr, double dt, std::vector<Geom*>& objects, 
                 std::vector<double>& part_share_arr, std::vector<int>& particle_counter, 
                 std::vector<double>& energies, std::vector<int>& elements_index_without_computing, 
                 std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, Eigen::VectorXd &potentials_array, 
                 double &electric_field_x, double &electric_field_y, double &electric_field_z,
                 std::vector<std::vector<double>> &result, std::vector<std::vector<double>> &pos_sec_part, std::vector<std::vector<double>> &vel_sec_part,
                 std::vector<std::vector<std::vector<double>>> &pos_sec_part_arr,
                 std::vector<std::vector<std::vector<double>>> &vel_sec_part_arr,
                 std::vector<std::vector<std::vector<double>>> &result_arr)
{
    traj(pos_part, vel0_part, time_arr, dt,objects, part_share_arr, particle_counter, energies,elements_index_without_computing, 
         vertex, tetra, potentials_array, electric_field_x, electric_field_y, 
         electric_field_z, result, pos_sec_part, vel_sec_part);

    std::vector<std::vector<double>> tmp_pos_sec_part_arr;
    std::vector<std::vector<double>> tmp_vel_sec_part_arr;
    std::vector<std::vector<std::vector<double>>> tmp_result_arr;

    std::vector<std::vector<double>> iter_pos_sec_part_arr;
    std::vector<std::vector<double>> iter_vel_sec_part_arr;
    std::vector<std::vector<std::vector<double>>> iter_result_arr;


    result_arr.push_back(result);
    iter_pos_sec_part_arr.insert(iter_pos_sec_part_arr.end(), pos_sec_part.begin(), pos_sec_part.end());
    iter_vel_sec_part_arr.insert(iter_vel_sec_part_arr.end(), vel_sec_part.begin(), vel_sec_part.end());
    // iter_result_arr.push_back(result);

    std::vector<std::vector<double>> tmp_pos_sec_part;
    std::vector<std::vector<double>> tmp_vel_sec_part;

    while (iter_pos_sec_part_arr.size() > 0) {
        std::cout << iter_pos_sec_part_arr.size() << std::endl;
        for (int i = 0; i < iter_pos_sec_part_arr.size(); i++) {
            result.clear();
            pos_sec_part.clear();
            vel_sec_part.clear();

            traj(iter_pos_sec_part_arr[i], iter_vel_sec_part_arr[i], time_arr, dt,objects,
                part_share_arr, particle_counter, energies,elements_index_without_computing,
                vertex, tetra, potentials_array,
                electric_field_x, electric_field_y, electric_field_z,
                result, tmp_pos_sec_part, tmp_vel_sec_part);
    
            tmp_pos_sec_part_arr.insert(tmp_pos_sec_part_arr.end(), pos_sec_part.begin(), pos_sec_part.end());
            tmp_vel_sec_part_arr.insert(tmp_vel_sec_part_arr.end(), vel_sec_part.begin(), vel_sec_part.end());
            result_arr.push_back(result);
        }
        iter_pos_sec_part_arr.clear();
        iter_vel_sec_part_arr.clear();

        iter_pos_sec_part_arr.insert(iter_pos_sec_part_arr.end(), tmp_pos_sec_part_arr.begin(), tmp_pos_sec_part_arr.end());
        iter_vel_sec_part_arr.insert(iter_vel_sec_part_arr.end(), tmp_vel_sec_part_arr.begin(), tmp_vel_sec_part_arr.end());

        tmp_pos_sec_part_arr.clear();
        tmp_vel_sec_part_arr.clear();
    }
}