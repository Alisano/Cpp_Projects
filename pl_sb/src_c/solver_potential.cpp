#include "solver_potential.hpp"

void dict_of_nodes_in_tetra(std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, std::vector<std::vector<int>> &adj_list) 
{
    std::vector<int> tmp;
    for (int i = 0; i < vertex.size(); i++) 
    {
        for (int j = 0; j < tetra.size(); j++) 
        {
            auto it = std::find(tetra[j].begin(), tetra[j].end(), i);
            if (it != tetra[j].end())
            {
                tmp.push_back(j);
            }        
        }
        adj_list.push_back(tmp);
        tmp.clear();
    }
}

void global_matrix_of_stiffness_computator_thread(std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, 
                                                  std::vector<std::vector<int>> &adj_list, Eigen::SparseMatrix<double> &global_matrix_of_stiffness, 
                                                  std::map<int, std::map<int, double>> &cache_gms, int i_begin, int i_end) 
{
    for (int i = i_begin; i < i_end; i++) 
    {
        for (int j = 0; j < vertex.size(); j++) 
        {
            std::vector<int> common_tetra;
            std::set_intersection(adj_list[i].begin(), adj_list[i].end(), adj_list[j].begin(), adj_list[j].end(), std::back_inserter(common_tetra));

            for (auto tetra_index : common_tetra) 
            {
                auto i_ind_it = std::find(begin(tetra[tetra_index]), end(tetra[tetra_index]), i);
                int i_index = std::distance(begin(tetra[tetra_index]), i_ind_it);
                auto j_ind_it = std::find(begin(tetra[tetra_index]), end(tetra[tetra_index]), j);
                int j_index = std::distance(begin(tetra[tetra_index]), j_ind_it);

                std::vector<std::vector<double>> triangle_points;
                for (auto vertex_index : tetra[tetra_index])
                {
                    triangle_points.push_back(vertex[vertex_index]);
                }

                Eigen::Matrix<double, 4, 4> volume_tetra_arr {
                    {1.0, triangle_points[0][0], triangle_points[0][1], triangle_points[0][2]},
                    {1.0, triangle_points[1][0], triangle_points[1][1], triangle_points[1][2]},
                    {1.0, triangle_points[2][0], triangle_points[2][1], triangle_points[2][2]},
                    {1.0, triangle_points[3][0], triangle_points[3][1], triangle_points[3][2]}
                };

                double volume_tetra = 1.0 / 6.0 * volume_tetra_arr.determinant();

                Eigen::Vector4d shape_func1_coef (0.0, 0.0, 0.0, 0.0);
                Eigen::Vector4d shape_func2_coef (0.0, 0.0, 0.0, 0.0);
                shape_func1_coef(i_index) = 1.0;
                shape_func2_coef(j_index) = 1.0;
                shape_func1_coef = volume_tetra_arr.completeOrthogonalDecomposition().solve(shape_func1_coef);
                shape_func2_coef = volume_tetra_arr.completeOrthogonalDecomposition().solve(shape_func2_coef);


                if (cache_gms.find(i) != cache_gms.end())
                {
                    if (cache_gms[i].find(j) != cache_gms[i].end())
                    {
                        cache_gms[i][j] += (shape_func1_coef[1] * shape_func2_coef[1] 
                                            + shape_func1_coef[2] * shape_func2_coef[2] 
                                            + shape_func1_coef[3] * shape_func2_coef[3]) * volume_tetra;
                    }
                    else
                    {
                        cache_gms[i][j] = (shape_func1_coef[1] * shape_func2_coef[1] 
                                            + shape_func1_coef[2] * shape_func2_coef[2] 
                                            + shape_func1_coef[3] * shape_func2_coef[3]) * volume_tetra;
                    }
                }
                else
                {
                    cache_gms[i][j] = (shape_func1_coef[1] * shape_func2_coef[1] 
                                        + shape_func1_coef[2] * shape_func2_coef[2] 
                                        + shape_func1_coef[3] * shape_func2_coef[3]) * volume_tetra;
                }
            }
        }
    }
}

void global_matrix_of_stiffness_computator_parallel(std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, 
                                                    std::vector<std::vector<int>> &adj_list, Eigen::SparseMatrix<double> &global_matrix_of_stiffness, const int CPU_COUNT)
{
    std::mutex mutex;
    int step_i = int(vertex.size() / CPU_COUNT);
    std::vector<std::thread> Threads;
    std::map<int, std::map<int, double>> cache_gms;
    
    for (int i = 0; i < vertex.size(); i += step_i)
    {
        if (int(i / step_i) == CPU_COUNT - 1) 
        {
            Threads.push_back(std::thread(global_matrix_of_stiffness_computator_thread, std::ref(vertex), std::ref(tetra), std::ref(adj_list), std::ref(global_matrix_of_stiffness), std::ref(cache_gms), i, vertex.size()));
            break;
        }
        Threads.push_back(std::thread(global_matrix_of_stiffness_computator_thread, std::ref(vertex), std::ref(tetra), std::ref(adj_list), std::ref(global_matrix_of_stiffness), std::ref(cache_gms), i, i + step_i));
    }

    for (int i = 0; i < Threads.size(); i++)
    {
        Threads[i].join();
    }

    for (const auto &external : cache_gms)
    {
        for(const auto & interntal : external.second)
        {
            global_matrix_of_stiffness.coeffRef(external.first, interntal.first) = interntal.second;
        }
    }
}

void global_matrix_boundary_condition_thread(std::vector<std::vector<double>> &vertex, std::vector<Geom*> &objects,  std::vector<bool> &chng_ind, int i_begin, int i_end) 
{
    std::vector<int> index(vertex.size());
    std::iota(std::begin(index), std::end(index), 0);

    for (int i = i_begin; i < i_end; i++) 
    {
        for (Geom* obj : objects) 
        {     
            if (obj->intersect_geom_xy(vertex[i])) 
            {
                chng_ind[i] = true;
            }
        }
    }
}

void global_matrix_boundary_condition_parallel(Eigen::SparseMatrix<double> &global_matrix_of_stiffness, std::vector<std::vector<double>> &vertex, std::vector<Geom*> &objects,  Eigen::VectorXd &potentials_array, const int CPU_COUNT)
{
    Eigen::VectorXd forces = Eigen::VectorXd::Zero(vertex.size());
    Eigen::VectorXd new_forces = Eigen::VectorXd::Zero(vertex.size());

    std::vector<std::thread> Threads;
    int step_i = int(vertex.size() / CPU_COUNT);
    // std::map<int, bool> chng_ind;
    std::vector<bool> chng_ind(vertex.size(), false);

    for (int i = 0; i < vertex.size(); i++) 
    {
        for (Geom* obj : objects) 
        {
            if (obj->intersect_geom_xy(vertex[i])) 
            {
                forces[i] = obj->potential;
            }
        }
    }
 
    new_forces = global_matrix_of_stiffness * forces;
    forces = -new_forces;

    for (int i = 0; i < vertex.size(); i++)
    {
        for (Geom* obj : objects)
        {
            if (obj->intersect_geom_xy(vertex[i])) 
            {
                forces[i] = obj->potential;
            }
        }
    }

    for (int i = 0; i < vertex.size(); i += step_i)
    {
        if (int(i / step_i) == CPU_COUNT - 1) {
            Threads.push_back(std::thread(global_matrix_boundary_condition_thread, std::ref(vertex), std::ref(objects), std::ref(chng_ind), i, vertex.size()));
            break;
        }

        Threads.push_back(std::thread(global_matrix_boundary_condition_thread, std::ref(vertex), std::ref(objects), std::ref(chng_ind), i, i + step_i));
    }

    for (int i = 0; i < Threads.size(); i++) 
    {
        Threads[i].join();
    }

    for (int i = 0; i < global_matrix_of_stiffness.outerSize(); i++)
    {
        for (Eigen::SparseMatrix<double>::InnerIterator it(global_matrix_of_stiffness, i); it; ++it)
        {
            if (it.row() != it.col())
            {
                if ((chng_ind[it.row()]) || (chng_ind[it.col()]))
                {
                    global_matrix_of_stiffness.coeffRef(it.row(), it.col()) = 0.0;
                    global_matrix_of_stiffness.coeffRef(it.col(), it.row()) = 0.0;
                }
            }
            it.row();
            it.col();
        }
    }

    for (int i = 0; i < vertex.size(); i++) {
        if (chng_ind[i]) 
        {
            global_matrix_of_stiffness.coeffRef(i, i) = 1.0;
        }
    }

    Eigen::ConjugateGradient<Eigen::SparseMatrix<double>, Eigen::Lower> solver;
    solver.compute(global_matrix_of_stiffness);
    potentials_array = solver.solve(forces);

}

void solver_potential(std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, std::vector<Geom*> &objects, Eigen::VectorXd &potentials_array) 
{
    Eigen::SparseMatrix<double> global_matrix_of_stiffness(vertex.size(), vertex.size());
    std::vector<std::vector<int>> adj_list;
    const int CPU_COUNT = 8;

    dict_of_nodes_in_tetra(vertex, tetra, adj_list);
    global_matrix_of_stiffness_computator_parallel(vertex, tetra, adj_list, global_matrix_of_stiffness, CPU_COUNT);
    global_matrix_boundary_condition_parallel(global_matrix_of_stiffness, vertex, objects, potentials_array, CPU_COUNT);
}