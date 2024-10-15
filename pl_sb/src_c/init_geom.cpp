#include "init_geom.hpp"

std::vector<Geom*> init_geom()
{
    double vac_height = 4e-6;
    double bias = 80e-6;
    double angle = 0.1;
    double width_cathode2 = 5e-6;
    double height_control2 = 2e-6;
    double cathode_control_gap = 5e-6;
    double electrodes_distance = 35e-6;
    double bar_el_dinode_displacement_y = 7e-6;
    double bar_el_dinode_displacement_x = 10e-6;
    double delta_V = 725;
    double dinode_V0 = 600;
    double control_potenitial = 350;
    double cathode_potential = 0;
    double downer_dinode_control_displacement_x = 20e-6;
    double downer_dinode_control_displacement_y = 35e-6;
    double bar_el_anode_displacement_x = 15.5e-6;
    double displacement_dinodes_x = -2.5e-6;
    double displacement_dinodes_y = 26e-6;
    double electrodes_height = 60e-6;
    double dinode_width = 30e-6;
    double dinode_height = 36e-6;
    double control_needle_width = 20e-6 ;
    double anode_y_displacement = -electrodes_height / 2 + 4e-6;
    double bar_el_height = 60e-6;
    double depth = 2e-6;

    Geom* control1 = new Quad(0.0, 0.0, 0.0, 140e-6, electrodes_height, depth, 0.0, 0.0);
    Geom* control_needle = new Quad(control1->xend, control1->yend - height_control2, 0.0, control_needle_width, height_control2, depth, angle, 0.0);
    double height_cathode2 = (electrodes_distance - control_needle->width * std::tan(angle) - cathode_control_gap / std::cos(angle));
    Geom* cathode1 =  new Quad(0.0, control1->yend + electrodes_distance, 0.0, control1->xend + control_needle->width, 48e-6, depth, 0.0 , 0.0);
    Geom* cathode_needle =  new Quad(cathode1->xend - width_cathode2, cathode1->y0 - height_cathode2, 0.0, width_cathode2, height_cathode2, depth, 0.0, 0.0);
    Geom* dinode1 = new Quad(control1->xend + downer_dinode_control_displacement_x, control_needle->y0 - downer_dinode_control_displacement_y - dinode_height, 0.0, dinode_width, dinode_height, depth, 0.0, 0.0);
    Geom* dinode2 =  new Quad(dinode1->xend + displacement_dinodes_x, displacement_dinodes_y + dinode1->height, 0.0, dinode1->width, dinode1->height, depth, 0.0, 0.0);
    Geom* bar1 = new Quad(dinode1->xend + bar_el_dinode_displacement_x, dinode1->yend - bar_el_height - bar_el_dinode_displacement_y, 0.0, 5e-6, bar_el_height, depth, 0.0, 0.0);
    Geom* bar0 = new Quad(dinode2->x0 - bar1->width - bar_el_dinode_displacement_x, dinode2->y0 + bar_el_dinode_displacement_y, 0.0, bar1->width, bar1->height, depth, 0.0, 0.0);
    Geom* anode = new Quad(bar1->xend + bar_el_anode_displacement_x, anode_y_displacement, 0.0 , 100e-6, electrodes_height, depth, 0.0, 0.0);

    Geom* dinode1_triangle = new Triangle(dinode1->x0, dinode1->yend, 0.0, dinode1->width, depth, 3.1415926 / 2, angle, 0.0);
    Geom* dinode2_triangle = new Triangle(dinode2->x0, dinode2->y0, 0.0, dinode2->width, depth, -3.1415926 / 2, - angle, 0.0);
    Geom* cathode_triangle = new Triangle(cathode_needle->x0, cathode_needle->y0, 0.0, width_cathode2, depth, -3.1415926 / 2, -angle, 0.0);

    cathode1->potential = cathode_potential;
    dinode1->potential = dinode_V0;
    control1->potential = control_potenitial;
    cathode_needle->potential = cathode1->potential;
    cathode_triangle->potential = cathode1->potential;
    dinode2->potential = dinode1->potential + delta_V;
    control_needle->potential = control1->potential;
    anode->potential = dinode1->potential + 2 * delta_V; 
    bar1->potential = dinode1->potential;
    bar0->potential = cathode1->potential;
    dinode1_triangle->potential = dinode1->potential;
    dinode2_triangle->potential = dinode2->potential;

    dinode1_triangle->is_emissive = true;
    dinode2_triangle->is_emissive = true;
    dinode1->is_emissive = true;
    dinode2->is_emissive = true;
    // control1->is_emissive = true;
    // control_needle->is_emissive = true;

    anode->energy_dist = true;
    anode->endobj = true;

    std::vector<Geom*> objects = {cathode1, cathode_needle, control1, control_needle, dinode1, dinode2, anode, bar1, bar0, dinode1_triangle, dinode2_triangle, cathode_triangle};

    double start_pos_x = cathode_needle->xend - width_cathode2 / 2.0;
    double start_pos_y = cathode_triangle->y0;
    double start_pos_z = depth / 2.0;

    // objects_dict = dict(zip(range(len(objects)), objects))
    // dinode1_triangle.SetUnionList(np.array([4, 9], dtype = np.int32))
    // dinode2_triangle.SetUnionList(np.array([5, 10], dtype = np.int32))
    // dinode1.SetUnionList(np.array([4, 9], dtype = np.int32))
    // dinode2.SetUnionList(np.array([5, 10], dtype = np.int32))
    // anode.SetUnionList(np.array([6], dtype = np.int32))
    // elements_index_without_computing = np.array([3], dtype = np.int32)
    return objects;
}