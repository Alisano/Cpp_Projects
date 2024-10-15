from src_py.libraries import *

# def vert_gen(bias, x_points, quads):
#     max_xend = max(quads, key = lambda x: x.xend).xend
#     max_yend = max(quads, key = lambda x: x.yend).yend
#     max_zend = max(quads, key = lambda x: x.zend).zend
#     aspect_ratio_xy = (max_yend + 2 * bias) / (max_xend + 2 * bias)
#     x_array = np.linspace(-bias, max_xend + bias, x_points)
#     y_array = np.linspace(-bias, max_yend + bias, int(aspect_ratio_xy * x_points))
#     z_array = np.linspace(-bias, max_zend + vac_height + bias, z_points)
#     y_array = list(reversed(y_array))
#     xy_vec = np.meshgrid(x_array, y_array)
#     dxdydz_vec = ((max_xend + 2 * bias)/ len(x_array), (max_yend + 2 * bias) / len(y_array), (max_zend + vac_height + 2 * bias) / len(z_array))
#     return dxdydz_vec, xy_vec, x_array, y_array, z_array

def Tri_cube_gen(Tri_cube, const):
    for i in range(0, 6, 2):
        Tri_cube.append(list(np.array((i + 1, i + 2, i + 3)) + const))
        Tri_cube.append(list(np.array((i + 1, i + 2, i)) + const))
    for i in range(2):
        Tri_cube.append(list(np.array((i, i + 2, i + 4)) + const))
        Tri_cube.append(list(np.array((i, i + 4, i + 6)) + const))
    Tri_cube.append(list(np.array((7, 0, 1)) + const))
    Tri_cube.append(list(np.array((7, 0, 6)) + const))
    return Tri_cube

def mesh_gen(objects, bias, depth, x_points, vac_height, z_points, steinerleft):
    cathode1, cathode_needle, control1, control_needle, dinode1, dinode2, anode, bar1, bar0, dinode1_triangle, dinode2_triangle, cathode_triangle = objects
    Nodes = []

    max_xend = max(objects, key = lambda x: x.xend).xend
    max_yend = max(objects, key = lambda x: x.yend).yend
    max_zend = max(objects, key = lambda x: x.zend).zend
    Nodes.append((-bias, -bias, -bias))
    Nodes.append((-bias, -bias, bias))
    Nodes.append((max_xend + bias, -bias, -bias))
    Nodes.append((max_xend + bias, -bias, bias))
    Nodes.append((max_xend + bias, max_yend + bias, -bias))
    Nodes.append((max_xend + bias, max_yend + bias, bias))
    Nodes.append((-bias, max_yend + bias, -bias))
    Nodes.append((-bias, max_yend + bias, bias))

    Tri = []
    Tri = Tri_cube_gen(Tri, 0)
    length_nodes_obj = len(Nodes)
    custom_objects = [dinode1, dinode2, anode, bar1, bar0]

    for obj in custom_objects:
        Nodes.append((obj.x0, obj.y0, 0))
        Nodes.append((obj.x0, obj.y0, depth))
        Nodes.append((obj.xend, obj.y0, 0))
        Nodes.append((obj.xend, obj.y0, depth))
        Nodes.append((obj.xend, obj.yend, 0))
        Nodes.append((obj.xend, obj.yend, depth))
        Nodes.append((obj.x0, obj.yend, 0))
        Nodes.append((obj.x0, obj.yend, depth))
        Tri_cube_gen(Tri, length_nodes_obj)
        length_nodes_obj = len(Nodes)

    aspect_ratio_xy = (max_yend + 2 * bias) / (max_xend + 2 * bias)

    dx = (max_xend + 2 * bias) / x_points
    dy = (max_yend + 2 * bias) / int(aspect_ratio_xy * x_points)
    dz = (max_zend + vac_height + 2 * bias) / z_points

    x_array = np.linspace(-bias + dx, max_xend + bias - dx, x_points)
    y_array = np.linspace(-bias + dy, max_yend + bias - dy, int(aspect_ratio_xy * x_points))
    z_array = np.linspace(-bias + dz, max_zend + vac_height + bias - dz, z_points)

    Nodes.append((control1.x[3], control1.y[3], 0))
    Nodes.append((control1.x[3], control1.y[3], depth))
    Nodes.append((control1.x[0], control1.y[0], 0))
    Nodes.append((control1.x[0], control1.y[0], depth))
    Nodes.append((control1.x[1], control1.y[1], 0))
    Nodes.append((control1.x[1], control1.y[1], depth))
    Nodes.append((control_needle.x[3], control_needle.y[3], 0))
    Nodes.append((control_needle.x[3], control_needle.y[3], depth))
    Nodes.append((control_needle.x[0], control_needle.y[0], 0))
    Nodes.append((control_needle.x[0], control_needle.y[0], depth))
    Nodes.append((control_needle.x[1], control_needle.y[1], 0))
    Nodes.append((control_needle.x[1], control_needle.y[1], depth))
    Nodes.append((control_needle.x[2], control_needle.y[2], 0))
    Nodes.append((control_needle.x[2], control_needle.y[2], depth))

    Tri.append(list(np.array((1, 2, 3)) + length_nodes_obj))
    Tri.append(list(np.array((1, 2, 0)) + length_nodes_obj))
    Tri.append(list(np.array((3, 4, 5)) + length_nodes_obj))
    Tri.append(list(np.array((3, 4, 2)) + length_nodes_obj))
    Tri.append(list(np.array((1, 6, 7)) + length_nodes_obj))
    Tri.append(list(np.array((1, 6, 0)) + length_nodes_obj))
    Tri.append(list(np.array((5, 8, 9)) + length_nodes_obj))
    Tri.append(list(np.array((5, 8, 4)) + length_nodes_obj))
    Tri.append(list(np.array((9, 6, 7)) + length_nodes_obj))
    Tri.append(list(np.array((9, 6, 8)) + length_nodes_obj))
    Tri.append(list(np.array((0, 8, 6)) + length_nodes_obj))
    Tri.append(list(np.array((8, 2, 4)) + length_nodes_obj))
    Tri.append(list(np.array((2, 8, 0)) + length_nodes_obj))
    Tri.append(list(np.array((1, 9, 7)) + length_nodes_obj))
    Tri.append(list(np.array((9, 3, 5)) + length_nodes_obj))
    Tri.append(list(np.array((3, 9, 1)) + length_nodes_obj))
    Tri.append(list(np.array((8, 12, 6)) + length_nodes_obj))
    Tri.append(list(np.array((8, 12, 10)) + length_nodes_obj))
    Tri.append(list(np.array((9, 13, 11)) + length_nodes_obj))
    Tri.append(list(np.array((9, 13, 9)) + length_nodes_obj))
    Tri.append(list(np.array((11, 12, 13)) + length_nodes_obj))
    Tri.append(list(np.array((11, 12, 10)) + length_nodes_obj))
    Tri.append(list(np.array((9, 10, 11)) + length_nodes_obj))
    Tri.append(list(np.array((9, 10, 8)) + length_nodes_obj))
    Tri.append(list(np.array((7, 12, 6)) + length_nodes_obj))
    Tri.append(list(np.array((7, 12, 13)) + length_nodes_obj))

    length_nodes_obj = len(Nodes)

    Nodes.append((cathode1.x[3], cathode1.y[3], 0))
    Nodes.append((cathode1.x[3], cathode1.y[3], depth))
    Nodes.append((cathode1.x[0], cathode1.y[0], 0))
    Nodes.append((cathode1.x[0], cathode1.y[0], depth))
    Nodes.append((cathode1.x[2], cathode1.y[2], 0))
    Nodes.append((cathode1.x[2], cathode1.y[2], depth))
    Nodes.append((cathode_needle.x[3], cathode_needle.y[3], 0))
    Nodes.append((cathode_needle.x[3], cathode_needle.y[3], depth))
    Nodes.append((cathode_needle.x[0], cathode_needle.y[0], 0))
    Nodes.append((cathode_needle.x[0], cathode_needle.y[0], depth))
    Nodes.append((cathode_needle.x[1], cathode_needle.y[1], 0))
    Nodes.append((cathode_needle.x[1], cathode_needle.y[1], depth))
    Nodes.append((cathode_needle.x[2], cathode_needle.y[2], 0))
    Nodes.append((cathode_needle.x[2], cathode_needle.y[2], depth))

    Tri.append(list(np.array((0, 6, 4)) + length_nodes_obj))
    Tri.append(list(np.array((2, 0, 6)) + length_nodes_obj))
    Tri.append(list(np.array((6, 4, 12)) + length_nodes_obj))
    Tri.append(list(np.array((1, 7, 3)) + length_nodes_obj))
    Tri.append(list(np.array((1, 7, 5)) + length_nodes_obj))
    Tri.append(list(np.array((7, 5, 13)) + length_nodes_obj))
    Tri.append(list(np.array((1, 4, 5)) + length_nodes_obj))
    Tri.append(list(np.array((1, 4, 0)) + length_nodes_obj))
    Tri.append(list(np.array((3, 0, 1)) + length_nodes_obj))
    Tri.append(list(np.array((3, 0, 2)) + length_nodes_obj))
    Tri.append(list(np.array((13, 4, 5)) + length_nodes_obj))
    Tri.append(list(np.array((13, 4, 12)) + length_nodes_obj))
    Tri.append(list(np.array((3, 6, 2)) + length_nodes_obj))
    Tri.append(list(np.array((3, 6, 7)) + length_nodes_obj))
    Tri.append(list(np.array((7, 12, 13)) + length_nodes_obj))
    Tri.append(list(np.array((7, 12, 6)) + length_nodes_obj))
    Tri.append(list(np.array((7, 8, 9)) + length_nodes_obj))
    Tri.append(list(np.array((7, 8, 6)) + length_nodes_obj))
    Tri.append(list(np.array((9, 10, 11)) + length_nodes_obj))
    Tri.append(list(np.array((9, 10, 8)) + length_nodes_obj))
    Tri.append(list(np.array((12, 11, 13)) + length_nodes_obj))
    Tri.append(list(np.array((12, 11, 10)) + length_nodes_obj))
    Tri.append(list(np.array((7, 11, 9)) + length_nodes_obj))
    Tri.append(list(np.array((7, 11, 13)) + length_nodes_obj))
    Tri.append(list(np.array((6, 10, 12)) + length_nodes_obj))
    Tri.append(list(np.array((6, 10, 8)) + length_nodes_obj))

    custom_objects = [dinode1_triangle, dinode2_triangle, cathode_triangle]
    length_nodes_obj = len(Nodes)
    Nodes.append((dinode1_triangle.x[2], dinode1_triangle.y[2], 0))
    Nodes.append((dinode1_triangle.x[2], dinode1_triangle.y[2], depth))
    Tri.append((length_nodes_obj + 1, 15, length_nodes_obj))
    Tri.append((14, 15, length_nodes_obj))
    Tri.append((length_nodes_obj, 13, 12))
    Tri.append((length_nodes_obj, 13, length_nodes_obj + 1))
    Tri.append((length_nodes_obj + 1, 13, 15))
    Tri.append((length_nodes_obj, 12, 14))

    length_nodes_obj = len(Nodes)
    Nodes.append((dinode2_triangle.x[2], dinode2_triangle.y[2], 0))
    Nodes.append((dinode2_triangle.x[2], dinode2_triangle.y[2], depth))
    Tri.append((length_nodes_obj + 1, 16, length_nodes_obj))
    Tri.append((length_nodes_obj + 1, 16, 17))
    Tri.append((length_nodes_obj + 1, 18, length_nodes_obj))
    Tri.append((length_nodes_obj + 1, 18, 19))
    Tri.append((length_nodes_obj, 18, 16))
    Tri.append((length_nodes_obj + 1, 17, 19))

    length_nodes_obj = len(Nodes)
    Nodes.append((cathode_triangle.x[2], cathode_triangle.y[2], 0))
    Nodes.append((cathode_triangle.x[2], cathode_triangle.y[2], depth))
    Tri.append((length_nodes_obj, 70, 72))
    Tri.append((length_nodes_obj + 1, 71, 73))
    Tri.append((length_nodes_obj + 1, 72, length_nodes_obj))
    Tri.append((length_nodes_obj + 1, 72, 73))
    Tri.append((length_nodes_obj + 1, 70, length_nodes_obj))
    Tri.append((length_nodes_obj + 1, 70, 71))

    for j in y_array:
        for i in x_array:
            for k in z_array:
                Nodes.append((i, j, k))

    tgen = tetgen.TetGen(np.array(Nodes), np.array(Tri))
    points, number_of_points_of_elements = tgen.tetrahedralize(mindihedral=0, minratio=1.1, steinerleft=steinerleft)
    return points, number_of_points_of_elements