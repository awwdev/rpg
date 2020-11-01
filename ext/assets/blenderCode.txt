import bpy
import bmesh
import os
import math

os.system('cls')

#file
user = os.environ['USERPROFILE']
path = str(user) + "\\Desktop\\grass2.mesh"
file = open(path, "w")

#object
#NEED TO HAVE A SELECTED OBJECT
bpy.ops.object.mode_set(mode = 'OBJECT')
ob = bpy.context.active_object
bpy.ops.object.duplicate()
tmp = bpy.context.active_object
bpy.ops.object.mode_set(mode = 'EDIT')
bpy.ops.mesh.select_all(action='SELECT')
bpy.ops.mesh.quads_convert_to_tris()
bpy.ops.object.mode_set(mode = 'OBJECT')

def fn_indices():
    faces = tmp.data.polygons
    s = "I\n"
    for f in faces:
        for i in f.vertices:
            s += str(i)
            s += ','
        s += '\n'
    return s

def fn_format(coord):
    s = str("%.6f" % coord) if math.copysign(1, coord) == -1 else str(" %.6f" % coord)
    return s

def fn_vertices():
    verts = tmp.data.vertices
    s = "V\n"
    i = 0
    for v in verts:
        #index
        s += str(i) + ",\t"
        i += 1
        #pos
        p = v.co
        x = fn_format(p[0]) + ','
        y = fn_format(p[1]) + ','
        z = fn_format(p[2]) + ','
        #index (should be same a iteration idx
        #index = str(v.index)+','
        #normal
        n = v.normal
        nx = fn_format(n[0]) + ','
        ny = fn_format(n[1]) + ','
        nz = fn_format(n[2]) + ','
        #sum
        s += x+y+z +nx+ny+nz + '\n'
    return s

def fn_face_normals():
    faces = tmp.data.polygons
    s = "F\n"
    i = 0
    for f in faces:
        #index
        s += str(i) + ",\t"
        i += 1
        #normal
        n = f.normal
        x = fn_format(n[0]) + ','
        y = fn_format(n[1]) + ','
        z = fn_format(n[2]) + ','
        #sum
        s += x+y+z + '\n'
    return s
    
def fn_vertex_colors():
    if not tmp.data.vertex_colors:
        return ""
    colors = tmp.data.vertex_colors[0].data
    s = "C\n"
    i = 0
    for c in colors:
        #index
        s += str(i) + ",\t"
        i += 1
        #color
        col = c.color
        r = fn_format(col[0]) + ','
        g = fn_format(col[1]) + ','
        b = fn_format(col[2]) + ','
        a = fn_format(col[3]) + ','
        #sum
        s += r+g+b+a + '\n'
    return s
    
str_indices = fn_indices()
str_vertices = fn_vertices()
str_face_normals = fn_face_normals()
str_colors = fn_vertex_colors()

print(str_indices)
print(str_colors)

#clean up
bpy.ops.object.delete()
bpy.context.view_layer.objects.active = ob 
ob.select_set(True)
     
#file
file.write(str_indices)    
file.write(str_vertices)    
file.write(str_face_normals)    
file.write(str_colors)   
file.close()