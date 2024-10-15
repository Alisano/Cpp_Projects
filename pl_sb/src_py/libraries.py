import numpy as np
from numba import njit, prange, float64, types, boolean, int32
from numba.experimental import jitclass
from scipy.constants import m_e, pi
from scipy.constants import elementary_charge as q
from numba.extending import as_numba_type
import math as m
import tetgen
import subprocess
import pyvista as pv
import matplotlib.pyplot as plt

# import numpy as np
# 
# 
# 
# import matplotlib.pyplot as plt
# from scipy.spatial import Delaunay
# from scipy import linalg

# from numba.typed import Dict
# from scipy import integrate
# from random import triangular
# 
# 
# from warnings import simplefilter
# from numba.core.errors import NumbaDeprecationWarning, NumbaPendingDeprecationWarning
# import pyvista as pv
# 
# import time
# from threading import Thread
# from mpl_toolkits.mplot3d import Axes3D
# from copy import copy
