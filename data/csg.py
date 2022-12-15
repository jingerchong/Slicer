import os
import numpy as np

# create folder for the meshes
if not os.path.exists("spot_meshes"):
    os.mkdir("spot_meshes")

# generate .scad
for roll in np.linspace(0, 180, num=13, endpoint=True):
    for pitch in np.linspace(0, 180, num=13, endpoint=True):
        file = open("spot.scad","w")

        file.write("rotate(a=[{:.2f},{:.2f},0]) {{ import(\"spot.stl\", convexity=1); }}".format(roll, pitch)) 
        file.close() 

        # output mesh
        os.system("openscad -o spot_meshes/spot_r_{:d}_p_{:d}.stl spot.scad".format(int(roll), int(pitch)))

