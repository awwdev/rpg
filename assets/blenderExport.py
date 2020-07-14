import bpy

current = bpy.context.object.data

file = open("C:/Users/Programmer/Desktop/wolve.txt", "w")

#VERTEX DATA
file.write("data,\n")
i = 0
while i < len (current.vertices):
    #POSITIONS
    #get coords
    x  = current.vertices[current.loops[i].vertex_index].co[0]
    y  = current.vertices[current.loops[i].vertex_index].co[1]
    z  = current.vertices[current.loops[i].vertex_index].co[2]
    #formate coords to specific count of decimal places
    xs = str("%.6f" % x)
    ys = str("%.6f" % y)
    zs = str("%.6f" % z)
    #align plus with minus values (beautify output)
    if x >= 0: xs = " " + xs
    if y >= 0: ys = " " + ys
    if z >= 0: zs = " " + zs
    #COLORS
    #get colors and formate colors to specific count of decimal places
    r  = str("%.6f" % current.vertex_colors[0].data[i].color[0])
    g  = str("%.6f" % current.vertex_colors[0].data[i].color[1])
    b  = str("%.6f" % current.vertex_colors[0].data[i].color[2])
    xn = current.vertices[current.loops[i].vertex_index].normal[0]
    yn = current.vertices[current.loops[i].vertex_index].normal[1]
    zn = current.vertices[current.loops[i].vertex_index].normal[2]
    #formate normal coords to specific count of decimal places
    xns = str("%.6f" % xn)
    yns = str("%.6f" % yn)
    zns = str("%.6f" % zn)
    #align plus with minus values (beautify output)
    if xn >= 0: xns = " " + xns
    if yn >= 0: yns = " " + yns
    if zn >= 0: zns = " " + zns
    #write
    file.write(
    xs + ","  + 
    ys + ","  + 
    zs + ", " + 
    r  + ","  +  
    g  + ","  +  
    b  + ", " +
    xns + "," +
    yns + "," +
    zns + 
    ",\n")
    #continue
    i += 1
    
#TRIANGLES
#triangulate faces before!
file.write("indices,\n")
i = 0
while i < len (current.polygons):
    #get indices
    #loop_indices
    i1 = str(current.polygons[i].loop_indices[0])
    i2 = str(current.polygons[i].loop_indices[1])
    i3 = str(current.polygons[i].loop_indices[2]) 
    #write
    file.write(i1 + "," + i2 + "," + i3 + ",\n")
    i += 1
    
file.close()