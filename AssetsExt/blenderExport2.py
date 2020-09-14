#SPLIT NORMAL VERSION !!!

#https://docs.blender.org/api/current/bpy.types.Mesh.html

import bpy
import os
from os import system, name 

system('cls') 
desktop = os.environ['USERPROFILE']
path = str(desktop) + "\\Desktop\\sword.txt"
print(path)

try:
    file = open(path, "w")
except IOError:
    print("IOError")
    
current = bpy.context.object.data
current.calc_normals_split()
print(current)

uv_layer = current.uv_layers.active.data


for poly in current.polygons:
    print("\nPolygon index: %d, length: %d\n" % (poly.index, poly.loop_total))
    for loop_index in poly.loop_indices:
        #print("\tVertex: %d" % current.loops[loop_index].vertex_index)
        index = current.loops[loop_index].vertex_index
        index_s = "{0:<5}".format(index)
        print(index_s)
        loop  = current.loops[loop_index]
        
        #POSITIONS
        x  = current.vertices[index].co[0]
        y  = current.vertices[index].co[1]
        z  = current.vertices[index].co[2]
        xs = str("%.6f" % x) #same decimal count
        ys = str("%.6f" % y)
        zs = str("%.6f" % z)
        if x >= 0: xs = " " + xs #beautify -/+ alignment
        if y >= 0: ys = " " + ys
        if z >= 0: zs = " " + zs
        print("\t",xs, ys, zs)
        
        #COLORS
        if len(current.vertex_colors) > 0:
            #color index is congruent to loop index 
            rs = str(" %.6f" % current.vertex_colors[0].data[loop_index].color[0])
            gs = str(" %.6f" % current.vertex_colors[0].data[loop_index].color[1])
            bs = str(" %.6f" % current.vertex_colors[0].data[loop_index].color[2])
            ts = str(" %.6f" % current.vertex_colors[0].data[loop_index].color[3])
            print("\t", rs, gs, bs, ts)
        else:
            rs = str(" %.6f" % 0)
            gs = str(" %.6f" % 0)
            bs = str(" %.6f" % 0)
            bs = str(" %.6f" % 1)
            print("\t", rs, gs, bs, ts)
        
        
        #NORMALS
        #current.vertices[index].normal[0] 
        nx = loop.normal[0] #use this normals when using custom normals
        ny = loop.normal[1]
        nz = loop.normal[2]
        nxs = str("%.6f" % nx) #same decimal count
        nys = str("%.6f" % ny)
        nzs = str("%.6f" % nz)
        if nx >= 0: nxs = " " + nxs #beautify -/+ alignment
        if ny >= 0: nys = " " + nys
        if nz >= 0: nzs = " " + nzs
        print("\t", nxs, nys, nzs)
        
        #UV
        ux = uv_layer[loop_index].uv[0]
        uy = uv_layer[loop_index].uv[1]
        uxs = str(" %.6f" % ux)
        uys = str(" %.6f" % uy)
        print("\t", uxs, uys)
 
        #write
        file.write(
        index_s + "," +
        xs  + ","  + 
        ys  + ","  + 
        zs  + ", " + 
        rs  + ","  +  
        gs  + ","  +  
        bs  + ","  +
        ts  + ", " +
        nxs + ","  +
        nys + ","  +
        nzs + ", " +
        uxs + ","  +
        uys + ",\n") 

        
file.close()