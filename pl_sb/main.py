from src_py.libraries import *
from src_py.geom_cls import *
from src_py.mesh import mesh_gen
    
define_verticles_on_object_edges = True
only_matplotlib_contour_for_potential = False
only_matplotlib_contour_for_electric_field = False
optimized_matrix_of_stiffness = True
optimize = True

vac_height = 4e-6
x_points = 15
z_points = 15
steinerleft = 1e4

bias = 80e-6
energy1 = 0
vel_x1 = 0
vel_y1 = -1
vel_z1 = 0
# dt = 1e-13
n_step = 300
angle = 0.1

width_cathode2 = 5e-6
height_control2 = 2e-6
cathode_control_gap = 5e-6
electrodes_distance = 35e-6
bar_el_dinode_displacement_y = 7e-6
bar_el_dinode_displacement_x = 10e-6
delta_V = 725
dinode_V0 = 600
control_potenitial = 350
cathode_potential = 0
downer_dinode_control_displacement_x = 20e-6
downer_dinode_control_displacement_y = 35e-6
bar_el_anode_displacement_x = 15.5e-6
displacement_dinodes_x = -2.5e-6
displacement_dinodes_y = 26e-6
electrodes_height = 60e-6
dinode_width = 30e-6
dinode_height = 36e-6
control_needle_width = 20e-6 
anode_y_displacement = -electrodes_height / 2 + 4e-6
bar_el_height = 60e-6
depth = 2e-6

control1 = InterfaceGeom.createQuad(0, 0, 0, 140e-6, electrodes_height, depth, 0)
control_needle = InterfaceGeom.createQuad(control1.xend, control1.yend - height_control2, 0, control_needle_width, height_control2, depth, angle)
height_cathode2 = electrodes_distance - control_needle.width * np.tan(angle) - cathode_control_gap / np.cos(angle)
cathode1 = InterfaceGeom.createQuad(0, control1.yend + electrodes_distance, 0, control1.xend + control_needle.width, 48e-6, depth, 0)
cathode_needle = InterfaceGeom.createQuad(cathode1.xend - width_cathode2, cathode1.y0 - height_cathode2, 0, width_cathode2, height_cathode2, depth, 0)
dinode1 = InterfaceGeom.createQuad(control1.xend + downer_dinode_control_displacement_x, control_needle.y0 - downer_dinode_control_displacement_y - dinode_height, 0, dinode_width, dinode_height, depth, 0)
dinode2 = InterfaceGeom.createQuad(dinode1.xend + displacement_dinodes_x, displacement_dinodes_y + dinode1.height, 0, dinode1.width, dinode1.height, depth, 0)
bar1 = InterfaceGeom.createQuad(dinode1.xend + bar_el_dinode_displacement_x, dinode1.yend - bar_el_height - bar_el_dinode_displacement_y, 0, 5e-6, bar_el_height, depth, 0)
bar0 = InterfaceGeom.createQuad(dinode2.x0 - bar1.width - bar_el_dinode_displacement_x, dinode2.y0 + bar_el_dinode_displacement_y, 0, bar1.width, bar1.height, depth, 0)
anode = InterfaceGeom.createQuad(bar1.xend + bar_el_anode_displacement_x, anode_y_displacement, 0, 100e-6, electrodes_height, depth, 0)

dinode1_triangle = InterfaceGeom.createTriangle(dinode1.x0, dinode1.yend, 0, dinode1.width, depth, pi / 2, angle, 0)
dinode2_triangle = InterfaceGeom.createTriangle(dinode2.x0, dinode2.y0, 0, dinode2.width, depth, -pi / 2, - angle, 0)
cathode_triangle = InterfaceGeom.createTriangle(cathode_needle.x0, cathode_needle.y0, 0, width_cathode2, depth, -pi / 2, -angle, 0)

x_pos1  = cathode_needle.xend - width_cathode2 / 2
y_pos1 = cathode_triangle.y0
z_pos1 = depth / 5.0
pos0_part1 = np.array([x_pos1, y_pos1, z_pos1])

cathode1.potential = cathode_potential
dinode1.potential = dinode_V0
control1.potential = control_potenitial
cathode_needle.potential = cathode1.potential
cathode_triangle.potential = cathode1.potential
dinode2.potential = dinode1.potential + delta_V
control_needle.potential = control1.potential
anode.potential = dinode1.potential + 2 * delta_V 
bar1.potential = dinode1.potential
bar0.potential = cathode1.potential
dinode1_triangle.potential = dinode1.potential
dinode2_triangle.potential = dinode2.potential

dinode1_triangle.is_emissive = True
dinode2_triangle.is_emissive = True
dinode1.is_emissive = True
dinode2.is_emissive = True

anode.energy_dist = True
anode.endobj = True

objects =  [cathode1, cathode_needle, control1, control_needle, dinode1,\
        dinode2, anode, bar1, bar0, dinode1_triangle, dinode2_triangle, cathode_triangle]

objects_dict = dict(zip(range(len(objects)), objects))

dinode1_triangle.SetUnionList(np.array([4, 9], dtype = np.int32))
dinode2_triangle.SetUnionList(np.array([5, 10], dtype = np.int32))
dinode1.SetUnionList(np.array([4, 9], dtype = np.int32))
dinode2.SetUnionList(np.array([5, 10], dtype = np.int32))
anode.SetUnionList(np.array([6], dtype = np.int32))

elements_index_without_computing = np.array([3], dtype = np.int32)

vert, tetra = mesh_gen(objects, bias, depth, x_points, vac_height, z_points, steinerleft)

cur_dir = "./src_py/output_py/"
name_of = "tetra.txt"
with open(f"{cur_dir}{name_of}", "w") as file:
    for i in range(len(tetra)):
        file.writelines(",".join(list(map(str, tetra[i]))))
        if i < (len(tetra)-1):
                file.write("\n")

name_of = "vertex.txt"
with open(f"{cur_dir}{name_of}", "w") as file:    
    for i in range(len(vert)):
        file.writelines(",".join(list(map(str, vert[i]))))
        if i < (len(vert)-1):
                file.write("\n")

path = "./"
file_name = "run.out"
subprocess.run(path + file_name)

def pyvista_graph(path, file_name):
    data = []
    with open(f"{path}{file_name}", "r") as file:    
        for line in file:
            data.append(float(line.replace("\n", "")))

    cells = np.insert(tetra, 0, 4, axis=1)
    mesh = pv.PolyData(vert, cells)
    pl = pv.Plotter(shape=(1, 1))
    pl.subplot(0, 0)
    celltypes = len(tetra) * [pv.CellType.TETRA]
    mesh = pv.UnstructuredGrid(cells, celltypes, vert)
    mesh.point_data['Point Data'] = data
    slices = mesh.slice_orthogonal(z = depth / 2)
    cmap = plt.cm.get_cmap("rainbow", 30)
    pl.add_mesh(slices, show_scalar_bar=True, show_edges=True, cmap=cmap)
    pl.show()

def coordinate_arr_gen(path, file_name):
    x = []
    y = []
    z = []
    traj_arr = []

    with open(f"{path}{file_name}", "r", encoding="utf-8") as file:
        for line in file:
            if line[0] == "\n":
                traj_arr.append([x, y, z])
                x = []
                y = []
                z = []
            else:
                tmp = list(map(float, line.replace("\n", "").split()))
                x.append(tmp[0])
                y.append(tmp[1])
                z.append(tmp[2])
    return traj_arr

def traj_graph(objects, traj_arr):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    for obj in objects:
        x = obj.x
        y = obj.y
        z1 = np.zeros(len(np.array(obj.x)), dtype=np.float64)
        z2 = np.ones(len(np.array(obj.x)), dtype=np.float64) * 2e-6
        plt.plot(x, y, z1, color = 'black')
        plt.plot(x, y, z2, color = 'black')
        x = np.array(list(zip(x,x))).reshape(-1)
        y = np.array(list(zip(y,y))).reshape(-1)
        z = np.array(list(zip(z1,z2))).reshape(-1)
        plt.plot(x, y, z, color = 'black')

    for coord in traj_arr:
        plt.plot(coord[0], coord[1], coord[2])

    plt.show()

path = "./src_c/output_c/"
file_name = "trajectories.txt"
traj_arr = coordinate_arr_gen(path, file_name)

file_name = "potentials.txt"
pyvista_graph(path, file_name)

file_name = "field.txt"
pyvista_graph(path, file_name)

file_name = "trajectories.txt"
traj_graph(objects, traj_arr)