#include "geom_cls.hpp"

double PI = 3.1415926;

std::vector<std::vector<double>> Quad::potential_mask(std::vector<std::vector<double>> phi, std::vector<double> dxdy_vec , double bias)
{
    for (int j = int((this->x0 + bias) / dxdy_vec[0]); j < int((this->xend + bias) / dxdy_vec[0]) + 1; j++) 
    {
        for (int i = int((this->y0 + bias) / dxdy_vec[1]); i < int((this-> yend + bias) / dxdy_vec[1]) + 1; i++)
        {
            if ((this->intersect_geom(bias, {i, j}, dxdy_vec)) && (i < phi.size()) && (j < phi[0].size())) 
            {
                phi[i][j] = this->potential;
            }
        } 
    }
    return phi;
}

bool Quad::intersect_geom_xy(std::vector<double> pos0_part)
{
    return (pos0_part[1] >= std::tan(this->angle) *(pos0_part[0] - x0) + y0) 
    && (pos0_part[1] <= std::tan(this->angle) * (pos0_part[0] - x0) + (y0 + this-> height))
    && ((pos0_part[0] >= x0) && (pos0_part[0] <= xend))
    && (pos0_part[2] >= z0) && (pos0_part[2] <= zend);
}

bool Quad::intersect_geom(double bias, std::vector<int> pos0_part_ji, std::vector<double> dxdy_vec)
{
    double tmp_xend = this->xend;
    double tmp_x0 = this->x0;
    double tmp_yend = this->yend;
    double tmp_y0 = this->y0;
    double tmp_height = this->height;
    double tmp_zend = this->zend;
    double tmp_z0 = this->z0;

    this->xend = ((this->xend + bias) / dxdy_vec[0]);
    this->x0 = ((this->x0 + bias) / dxdy_vec[0]); 
    this->yend = ((this->yend + bias)/  dxdy_vec[1]); 
    this->y0 = ((this->y0 + bias) / dxdy_vec[1]); 
    this->height = ((this->height + bias) / dxdy_vec[1]); 
    this->zend = ((this->zend + bias) / dxdy_vec[2]); 
    this->z0 = ((this->z0 + bias) / dxdy_vec[2]);

    std::vector<double> double_ji_pos(pos0_part_ji.begin() , pos0_part_ji.end());
    bool result = this->intersect_geom_xy(double_ji_pos);

    this->xend = tmp_xend;
    this->x0 = tmp_x0;
    this->yend = tmp_yend;
    this->y0 = tmp_y0;
    this->height = tmp_height;
    this->zend = tmp_zend;
    this->z0 = tmp_z0;

    return result;
}

std::vector<std::vector<double>> Triangle::potential_mask(std::vector<std::vector<double>> phi, std::vector<double> dxdy_vec, double bias)
{
    for (int j = int((this->x0 + bias) / dxdy_vec[0]); j < int((this->xend + bias) / dxdy_vec[0]) + 1; j++) 
    {
        for (int i = int((this->y0 + bias) / dxdy_vec[1]); i < int((this-> yend + bias) / dxdy_vec[1]) + 1; i++)
        {
            if ((this->intersect_geom(bias, {i, j}, dxdy_vec)) && (i < phi.size()) && (j < phi[0].size())) 
            {
                phi[i][j] = this->potential;
            }
        } 
    }
    return phi;
}

bool Triangle::intersect_geom_xy(std::vector<double> pos0_part)
{
    if ((pos0_part[2] >= this->z0) && (pos0_part[2] <= this->zend)) 
    {
        if ((this->angle_left)== (PI / 2))
        {
            if (this->angle_left < 0)
            {
                return ((pos0_part[1]) <= (this-> yend)
                && (pos0_part[1] >= std::tan(this->angle_right * (-1)) * (pos0_part[0] - (this->xend)) + this->yend )
                && (pos0_part[0] >= this->x0) && (pos0_part[0] <= this->xend));
            } 
            else 
            {
                return (pos0_part[1] >= this-> y0)
                && (pos0_part[1] <= std::tan(PI - (this->angle_right))* (pos0_part[0] - (this->xend)) + this->y0 )
                && (pos0_part[0] >= this->x0) && (pos0_part[0] <= this->xend);
            }
        }
        else if (abs(this->angle_right) == (PI / 2.0)) 
        {
            if (this->angle_right < 0){
                return ((pos0_part[1]) >= std::tan(this->angle_left * -1) * (pos0_part[0] - (this->x0)) + this->yend
                && (pos0_part[1]  <= this->yend )
                && (pos0_part[0] >= this->x0) && (pos0_part[0] <= this->xend));
            } 
            else 
            {
                return (pos0_part[1] <=  std::tan(this->angle_left) * (pos0_part[0] - this->x0) + this->y0
                && (pos0_part[1] >= this->y0 )
                && (pos0_part[0] >= this->x0) && (pos0_part[0] <= this->xend));
            }

        } 
        else 
        {
            return ((pos0_part[1] <= std::tan(this->angle_left) * (pos0_part[0] - this->x0) + this->y0)
            && (pos0_part[1] <= std::tan(PI - (this->angle_right) * (pos0_part[0] - this->xend) + this->y0)
            && (pos0_part[0] >= this->x0) && (pos0_part[0] <= this->xend)));
        }
    } 
    else 
    {
        return false;
    }
}

bool Triangle::intersect_geom(double bias, std::vector<int> pos0_part_ji, std::vector<double> dxdy_vec)
{
    double tmp_xend = this->xend;
    double tmp_x0 = this->x0;
    double tmp_yend = this->yend;
    double tmp_y0 = this->y0;
    double tmp_zend = this->zend;
    double tmp_z0 = this->z0;

    this->xend = ((this->xend + bias) / dxdy_vec[0]);
    this->x0 = ((this->x0 + bias) / dxdy_vec[0]); 
    this->yend = ((this->yend + bias)/  dxdy_vec[1]); 
    this->y0 = ((this->y0 + bias) / dxdy_vec[1]); 
    this->zend = ((this->zend + bias) / dxdy_vec[2]); 
    this->z0 = ((this->z0 + bias) / dxdy_vec[2]);

    std::vector<double> double_ji_pos(pos0_part_ji.begin() , pos0_part_ji.end());
    bool result = this->intersect_geom_xy(double_ji_pos);

    this->xend = tmp_xend;
    this->x0 = tmp_x0;
    this->yend = tmp_yend;
    this->y0 = tmp_y0;
    this->zend = tmp_zend;
    this->z0 = tmp_z0;

    return result;
}

Triangle::Triangle(double x0, double y0, double z0, double width, double depth, double angle_left, double angle_right, double potential) 
{
    potential = 0.0;
    this->height = width / (std::cos(angle_right) / std::sin(angle_right) + std::cos(angle_left) / std::sin(angle_left));
    this->x = {x0, x0 + width, x0 + width - std::cos(angle_right) / std::sin(angle_right) * this->height, x0};
    this->y = {y0, y0, y0 + this->height, y0};
    this->x0 = *std::min_element(begin(this->x), end(this->x));
    this->y0 = *std::min_element(begin(this->y), end(this->y));
    this->z0 = z0;
    this->xend = *std::max_element(begin(this->x), end(this->x));
    this->yend = *std::max_element(begin(this->y), end(this->y));
    this->zend = z0 + depth;
    this->potential = potential;
    this->angle_right = angle_right;
    this->angle_left = angle_left;
    this->name = "Triangle";
    this->width = width;
    this->depth = depth;
    this->is_emissive = false;
    this->energy_dist = false;
    this->endobj = false;
    this->twin = false;
}

Quad::Quad(double x0, double y0, double z0, double width, double height, double depth, double angle, double potential) 
{
    potential = 0.0;
    this->x ={x0, x0 + width, x0 + width, x0, x0};
    this->y ={y0, y0 + width * std::tan(angle), y0 + width * std::tan(angle) + height, y0 + height, y0};
    this->x0 = x0;
    this->y0 = y0;
    this->z0 = z0;
    this->xend = *std::max_element(begin(this->x), end(this->x));
    this->yend = *std::max_element(begin(this->y), end(this->y));
    this->zend = z0 + depth;
    this->potential = potential;
    this->width = width;
    this->height = height;
    this->depth = depth;
    this->angle = angle;
    this->name = "Quad";
    this->is_emissive = false;
    this->energy_dist = false;
    this->endobj = false;
    this->twin = false;
}