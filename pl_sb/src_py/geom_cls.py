from src_py.libraries import *

spec1 = [
    ('x0', float64),
    ('y0', float64),
    ('z0', float64),
    ('xend', float64),
    ('yend', float64),
    ('zend', float64),
    ('width', float64),
    ('height', float64),
    ('depth', float64),
    ('angle', float64),
    ('potential', float64),
    ('x', float64[:]),
    ('y', float64[:]),
    ('name', types.string),
    ('pos0_part', float64[:]),
    ('is_emissive', boolean),
    ('energy_dist', boolean),
    ('endobj', boolean),
    ('result', boolean),
    ('union', boolean),
]

@jitclass(spec1)
class Quad_numba:
    def __init__(self, x0, y0, z0, width, height, depth, angle, potential = 0):
        self.x = np.array([x0, x0 + width, x0 + width, x0, x0])
        self.y = np.array([y0, y0 + width * np.tan(angle), y0 + width * np.tan(angle) + height, y0 + height, y0])
        self.x0 = x0
        self.y0 = y0
        self.z0 = z0
        self.xend = max(self.x)
        self.yend = max(self.y)
        self.zend = z0 + depth
        self.potential = potential
        self.width = width
        self.height = height
        self.depth = depth
        self.angle = angle
        self.name = 'Quad'
        self.is_emissive = False
        self.energy_dist = False
        self.endobj = False
        self.union = False
    
    def __eq__(self, __value) -> bool:
        return self.name == __value.__str__
    
    def __str__(self) -> str:
        return self.name

    def potential_mask(self, phi, dxdy_vec, bias):
        for j in range(int((self.x0 + bias)/ dxdy_vec[0]), int((self.xend + bias) / dxdy_vec[0]) + 1):
            for i in range(int((self.y0 + bias) / dxdy_vec[1]), int((self.yend + bias) / dxdy_vec[1]) + 1):
                if (self.intersect_mask_quad(bias, (j, i), dxdy_vec) and (i < phi.shape[0]) and (j < phi.shape[1])):
                    phi[i][j] = self.potential
        return phi
    
    def intersect_mask_quad(self, bias, pos0_part_ji, dxdy_vec):

        tmp_xend = self.xend
        tmp_x0 = self.x0
        tmp_yend = self.yend
        tmp_y0 = self.y0
        tmp_height = self.height
        tmp_zend = self.zend
        tmp_z0 = self.z0

        self.xend = ((self.xend + bias) / dxdy_vec[0])
        self.x0 = ((self.x0 + bias) / dxdy_vec[0])
        self.yend = ((self.yend + bias) / dxdy_vec[1])
        self.y0 =  ((self.y0 + bias) / dxdy_vec[1])
        self.height = (self.height / dxdy_vec[1])
        self.zend = ((self.yend + bias) / dxdy_vec[2])
        self.z0 =  ((self.y0 + bias) / dxdy_vec[2])
        
        result = self.intersect_mask_quad_xy(pos0_part_ji)

        self.xend = tmp_xend
        self.x0 = tmp_x0
        self.yend = tmp_yend
        self.y0 = tmp_y0
        self.height = tmp_height
        self.yend = tmp_zend
        self.y0 = tmp_z0

        return result
    
    def intersect_mask_quad_xy(self, pos0_part):
        return (pos0_part[1] >= np.tan(self.angle) * (pos0_part[0] - self.x0) + self.y0) \
        and (pos0_part[1] <= np.tan(self.angle) * (pos0_part[0] - self.x0) + (self.y0 + self.height)) \
        and (pos0_part[0] >= self.x0) and (pos0_part[0] <= self.xend) \
        and (pos0_part[2] >= self.z0) and (pos0_part[2] <= self.zend)
    
spec2 = [
    ('x0', float64),
    ('y0', float64),
    ('z0', float64),
    ('xend', float64),
    ('yend', float64),
    ('zend', float64),
    ('width', float64),
    ('angle_left', float64),
    ('angle_right', float64),
    ('potential', float64),
    ('x', float64[:]),
    ('y', float64[:]),
    ('name', types.string),
    ('height', float64),
    ('depth', float64),
    ('pos0_part', float64[:]),
    ('is_emissive', boolean),
    ('energy_dist', boolean),
    ('endobj', boolean),
    ('result', boolean),
    ('union', boolean),
]

@jitclass(spec2)
class Triangle_numba:
    def __init__(self, x0, y0, z0, width, depth, angle_left, angle_right, potential = 0):
        self.height = width / (np.cos(angle_right) / np.sin(angle_right) + np.cos(angle_left) / np.sin(angle_left))
        self.x = np.array([x0, x0 + width, x0 + width - np.cos(angle_right) / np.sin(angle_right) * self.height, x0])
        self.y = np.array([y0, y0, y0 + self.height, y0])
        self.x0 = min(self.x)
        self.y0 = min(self.y)
        self.z0 = z0
        self.xend = max(self.x)
        self.yend = max(self.y)
        self.zend = z0 + depth
        self.potential = potential
        self.angle_right = angle_right
        self.angle_left = angle_left
        self.name = 'Triangle'
        self.width = width
        self.depth = depth
        self.is_emissive = False
        self.energy_dist = False
        self.endobj = False
        self.union = False

    def __eq__(self, __value) -> bool:
        return self.name == __value.__str__
    
    def __str__(self) -> str:
        return self.name

    def potential_mask(self, phi, dxdy_vec, bias):
        for j in range(int((self.x0 + bias)/ dxdy_vec[0]), int((self.xend + bias) / dxdy_vec[0]) + 1):
            for i in range(int((self.y0 + bias) / dxdy_vec[1]), int((self.yend + bias) / dxdy_vec[1]) + 1):
                if (self.intersect_mask_quad(bias, (j, i), dxdy_vec) and (i < phi.shape[0]) and (j < phi.shape[1])):
                    phi[i][j] = self.potential
        return phi

    def intersect_mask_quad(self, bias, pos0_part_ij, dxdy_vec):

        tmp_xend = self.xend
        tmp_x0 = self.x0
        tmp_yend = self.yend
        tmp_y0 = self.y0
        tmp_zend = self.zend
        tmp_z0 = self.z0

        self.xend = ((self.xend + bias) / dxdy_vec[0])
        self.x0 = ((self.x0+ bias) / dxdy_vec[0])
        self.yend = ((self.yend + bias) / dxdy_vec[1])
        self.y0 =  ((self.y0 + bias) / dxdy_vec[1])
        self.zend = ((self.yend + bias) / dxdy_vec[2])
        self.z0 =  ((self.y0 + bias) / dxdy_vec[2])

        result = self.intersect_mask_quad_xy(pos0_part_ij)

        self.xend = tmp_xend
        self.x0 = tmp_x0
        self.yend = tmp_yend
        self.y0 = tmp_y0
        self.yend = tmp_zend
        self.y0 = tmp_z0

        return result

    def intersect_mask_quad_xy(self, pos0_part):
        if ((pos0_part[2] >= self.z0) and (pos0_part[2] <= self.zend)):
            if (abs(self.angle_left) == (pi / 2)) :
                if (self.angle_left < 0):

                    return ((pos0_part[1] <= self.yend) \
                    and (pos0_part[1] >= (np.tan(-self.angle_right) * (pos0_part[0] - self.xend) + self.yend)) \
                    and (pos0_part[0] >= self.x0) and (pos0_part[0] <= self.xend))
                else:
                    return ((pos0_part[1] >= self.y0) \
                    and (pos0_part[1] <= (np.tan(pi - self.angle_right) * (pos0_part[0] - self.xend) + self.y0)) \
                    and (pos0_part[0] >= self.x0) and (pos0_part[0] <= self.xend))
                
            elif (abs(self.angle_right) == (pi / 2)) :

                if (self.angle_right < 0):
                    return ((pos0_part[1] >= (np.tan(self.angle_left) * (pos0_part[0] - self.x0) + self.yend)) \
                    and (pos0_part[1] <= self.yend) \
                    and (pos0_part[0] >= self.x0) and (pos0_part[0] <= self.xend))
                else:
                    return ((pos0_part[1] <= (np.tan(self.angle_left) * (pos0_part[0] - self.x0) + self.y0)) \
                    and (pos0_part[1] >= self.y0) \
                    and (pos0_part[0] >= self.x0) and (pos0_part[0] <= self.xend))
                
            else:
                
                return ((pos0_part[1] <= (np.tan(self.angle_left) * (pos0_part[0] - self.x0) + self.y0)) \
                and (pos0_part[1] <= (np.tan(pi - self.angle_right) * (pos0_part[0] - self.xend) + self.y0)) \
                and (pos0_part[0] >= self.x0) and (pos0_part[0] <= self.xend))
        else:
            return False

spec3 = [
    ('x0', float64),
    ('y0', float64),
    ('z0', float64),
    ('xend', float64),
    ('yend', float64),
    ('zend', float64),
    ('width', float64),
    ('depth', float64),
    ('angle_left', float64),
    ('angle_right', float64),
    ('potential', float64),
    ('x', float64[:]),
    ('y', float64[:]),
    ('name', types.string),
    ('height', float64),
    ('pos0_part', float64[:]),
    ('is_emissive', boolean),
    ('height', float64),
    ('angle', float64),
    ('potential', float64),
    ('quad_obj', as_numba_type(Quad_numba)),
    ('triangle_obj', as_numba_type(Triangle_numba)),
    ('number_of_union_obj_in_obj_list', int32[:]),
    ('energy_dist', boolean),
    ('endobj', boolean),
    ('argobj', float64[:]),
    ('union', boolean),
]

@jitclass(spec3)
class InterfaceGeom:
    def __init__(self, name, argobj):
        if (name == "Quad"):
            quad_obj = Quad_numba(argobj[0], argobj[1], argobj[2], argobj[3], argobj[4], argobj[5], argobj[6], argobj[7])
            self.quad_obj = quad_obj
            self.potential = quad_obj.potential
            self.x0 = quad_obj.x0 
            self.y0 = quad_obj.y0
            self.z0 = quad_obj.z0
            self.x = quad_obj.x
            self.y = quad_obj.y
            self.xend = quad_obj.xend 
            self.yend = quad_obj.yend
            self.zend = quad_obj.zend
            self.width = quad_obj.width
            self.depth = quad_obj.depth
            self.height = quad_obj.height
            self.angle = quad_obj.angle
            self.is_emissive = quad_obj.is_emissive
            self.union = quad_obj.union
        else:
            triangle_obj = Triangle_numba(argobj[0], argobj[1], argobj[2], argobj[3], argobj[4], argobj[5], argobj[6], argobj[7])
            self.triangle_obj = triangle_obj
            self.potential = triangle_obj.potential
            self.x0 = triangle_obj.x0
            self.y0 = triangle_obj.y0
            self.z0 = triangle_obj.z0
            self.x = triangle_obj.x
            self.y = triangle_obj.y
            self.xend = triangle_obj.xend 
            self.yend = triangle_obj.yend
            self.zend = triangle_obj.zend
            self.width = triangle_obj.width
            self.depth = triangle_obj.depth
            self.angle_left = triangle_obj.angle_left 
            self.angle_right = triangle_obj.angle_right
            self.union = triangle_obj.union
            self.is_emissive = triangle_obj.is_emissive
            self.height = triangle_obj.width / (np.cos(triangle_obj.angle_right) / np.sin(triangle_obj.angle_right) \
                                                + np.cos(triangle_obj.angle_left) / np.sin(triangle_obj.angle_left))

        self.energy_dist = False
        self.endobj = False
        self.name = name

    @staticmethod
    def createTriangle(x0, y0, z0, width, depth, angle_left, angle_right, potential = 0):
        name = 'Triangle'
        argobj = [x0, y0, z0, width, depth, angle_left, angle_right, potential]
        return InterfaceGeom(name, argobj)

    @staticmethod
    def createQuad(x0, y0, z0, width, height, depth, angle, potential = 0):
        name = 'Quad'
        argobj = [x0, y0, z0, width, height, depth, angle, potential]
        return InterfaceGeom(name, argobj)
    
    def potential_mask(self, phi, dxdy_vec, bias):
        if ((self.name == "Quad")):
            self.quad_obj.potential = self.potential
            return self.quad_obj.potential_mask(phi, dxdy_vec, bias)
        else:
            self.triangle_obj.potential = self.potential
            return self.triangle_obj.potential_mask(phi, dxdy_vec, bias)

    def intersect_mask_quad(self, bias, pos0_part_ij, dxdy_vec):
        if ((self.name == "Quad")):
            return self.quad_obj.intersect_mask_quad(bias, pos0_part_ij, dxdy_vec)
        else:
            return self.triangle_obj.intersect_mask_quad(bias, pos0_part_ij, dxdy_vec)
    
    def intersect_mask_quad_xy(self, pos0_part):
        if ((self.name == "Quad")):
            return self.quad_obj.intersect_mask_quad_xy(pos0_part)
        else:
            return self.triangle_obj.intersect_mask_quad_xy(pos0_part)
    
    def SetUnionList(self, number_of_union_obj_in_obj_list):
        self.number_of_union_obj_in_obj_list = number_of_union_obj_in_obj_list
        self.union = True
    
    def GetUnionList(self):
        return self.number_of_union_obj_in_obj_list
    
    def __eq__(self, __value) -> bool:
        return self.name == __value.__str__
    
    def __str__(self) -> str:
        return self.name