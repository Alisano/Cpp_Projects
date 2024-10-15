#include "solver_field.hpp"

void arr_for_triple_product(std::vector<double> &pos, std::vector<double> &point1, std::vector<double> &point2, std::vector<double> &point3, Eigen::Matrix3d &array_p)
{
    array_p << point2[0] - point1[0], point2[1] - point1[1], point2[2] - point1[2], 
               point3[0] - point1[0], point3[1] - point1[1], point3[2] - point1[2], 
               pos[0] - point1[0], pos[1] - point1[1], pos[2] - point1[2];
}

bool point_tetra(std::vector<double> &pos, std::vector<std::vector<double>> &tetra_coord)
{
    Eigen::Matrix3d array_p1;
    Eigen::Matrix3d array_p2;
    Eigen::Matrix3d array_p3;
    Eigen::Matrix3d array_p4;

    arr_for_triple_product(pos, tetra_coord[0], tetra_coord[1], tetra_coord[2], array_p1);
    arr_for_triple_product(pos, tetra_coord[0], tetra_coord[3], tetra_coord[1], array_p2);
    arr_for_triple_product(pos, tetra_coord[0], tetra_coord[2], tetra_coord[3], array_p3);
    arr_for_triple_product(pos, tetra_coord[3], tetra_coord[2], tetra_coord[1], array_p4);

    double res1 = array_p1.determinant();
    double res2 = array_p2.determinant();
    double res3 = array_p3.determinant();
    double res4 = array_p4.determinant();

    bool has_neg = ((res1 <= 0) && (res2 <= 0) && (res3 <= 0) && (res4 <= 0));
    bool has_pos = ((res1 >= 0) && (res2 >= 0) && (res3 >= 0) && (res4 >= 0));

    return (has_neg || has_pos);
}

void E_field_coef(std::vector<double> &pos, int coef_num, std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, Eigen::VectorXd &potentials_array, double &electric_field)
{
    std::vector<std::vector<double>> tetra_coord;
    std::vector<double> el_potentials;
    
    for (auto ind_of_tetra : tetra) {
        tetra_coord = {};
        for (auto ind_point : ind_of_tetra) {
            tetra_coord.push_back(vertex[ind_point]);
        }
        if (point_tetra(pos, tetra_coord))
        {
            for (auto ind_point : ind_of_tetra) {
                el_potentials.push_back(potentials_array(ind_point));
            }
            
            Eigen::Matrix<double, 4, 4> volume_tetra_arr {
                {1, tetra_coord[0][0], tetra_coord[0][1], tetra_coord[0][2]},
                {1, tetra_coord[1][0], tetra_coord[1][1], tetra_coord[1][2]},
                {1, tetra_coord[2][0], tetra_coord[2][1], tetra_coord[2][2]},
                {1, tetra_coord[3][0], tetra_coord[3][1], tetra_coord[3][2]}
            };
            
            Eigen::Vector4d shape_func1_coef = volume_tetra_arr.completeOrthogonalDecomposition().solve(Eigen::Vector4d(1.0, 0.0, 0.0, 0.0));
            Eigen::Vector4d shape_func2_coef = volume_tetra_arr.completeOrthogonalDecomposition().solve(Eigen::Vector4d(0.0, 1.0, 0.0, 0.0));
            Eigen::Vector4d shape_func3_coef = volume_tetra_arr.completeOrthogonalDecomposition().solve(Eigen::Vector4d(0.0, 0.0, 1.0, 0.0));
            Eigen::Vector4d shape_func4_coef = volume_tetra_arr.completeOrthogonalDecomposition().solve(Eigen::Vector4d(0.0, 0.0, 0.0, 1.0));

            electric_field = -1.0 * (shape_func1_coef[coef_num] * el_potentials[0]
                                    + shape_func2_coef[coef_num] * el_potentials[1]
                                    + shape_func3_coef[coef_num] * el_potentials[2]
                                    + shape_func4_coef[coef_num] * el_potentials[3]);
            return;
        }
    }
}

void E_field_x(std::vector<double> &pos, std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, Eigen::VectorXd &potentials_array, double &electric_field_x)
{
    E_field_coef(pos, 1, vertex, tetra, potentials_array, electric_field_x);
}

void E_field_y(std::vector<double> &pos, std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, Eigen::VectorXd &potentials_array, double &electric_field_y)
{
    E_field_coef(pos, 2, vertex, tetra, potentials_array, electric_field_y);
}

void E_field_z(std::vector<double> &pos, std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, Eigen::VectorXd &potentials_array, double &electric_field_z)
{
    E_field_coef(pos, 3, vertex, tetra, potentials_array, electric_field_z);
}

double E_field_mod(std::vector<double> &pos, std::vector<std::vector<double>> &vertex, std::vector<std::vector<int>> &tetra, 
                    Eigen::VectorXd &potentials_array, double &electric_field_x,  double &electric_field_y, double &electric_field_z)
{
    E_field_x(pos, vertex, tetra, potentials_array, electric_field_x);
    E_field_y(pos, vertex, tetra, potentials_array, electric_field_y);
    E_field_z(pos, vertex, tetra, potentials_array, electric_field_z);

    double E_field = pow(electric_field_x, 2.0) +
                     pow(electric_field_y, 2.0) +
                     pow(electric_field_z, 2.0);
    return sqrt(E_field);
}