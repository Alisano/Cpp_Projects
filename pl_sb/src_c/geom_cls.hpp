#pragma once
#ifndef GEOM_H
#define GEOM_H
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

class Geom
{
public:
    double x0;
    double y0;
    double z0;
    double width;
    double potential;
    std::vector<double> x;
    std::vector<double> y;
    double xend;
    double zend;
    double yend;
    bool energy_dist;
    std::vector<double> pos0_part;
    bool is_emissive;
    bool twin;
    bool result;
    bool endobj;
    std::string name;
    double depth;
    double angle;
    std::vector<int> number_of_union_obj_in_obj_list;
    double argobj;
    double height;
    double angle_left;
    double angle_right;


    virtual std::vector<std::vector<double>> potential_mask(std::vector<std::vector<double>> phi, std::vector<double> dxdy_vec , double bias) = 0;
    virtual bool intersect_geom_xy(std::vector<double> pos0_part) = 0;
    virtual bool intersect_geom(double bias, std::vector<int> pos0_part_ji, std::vector<double> dxdy_vec) = 0;
};

class Quad : public Geom
{
    public:
        Quad(double x0, double y0,double z0,double width,double height,double depth,double angle,double potential);
        std::vector<std::vector<double>> potential_mask(std::vector<std::vector<double>> phi, std::vector<double> dxdy_vec , double bias);
        bool intersect_geom_xy(std::vector<double> pos0_part);
        bool intersect_geom(double bias, std::vector<int> pos0_part_ji, std::vector<double> dxdy_vec);
};

class Triangle : public Geom
{
    public:
        Triangle(double x0, double y0, double z0, double width, double depth, double angle_left, double angle_right, double potential);
        std::vector<std::vector<double>> potential_mask(std::vector<std::vector<double>> phi, std::vector<double> dxdy_vec , double bias);
        bool intersect_geom_xy(std::vector<double> pos0_part);
        bool intersect_geom(double bias, std::vector<int> pos0_part_ji, std::vector<double> dxdy_vec);
};

#endif
