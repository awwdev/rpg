glslc ui.vert -o spv/ui.vert.spv
glslc ui.frag -o spv/ui.frag.spv

glslc post.vert -o spv/post.vert.spv
glslc post.frag -o spv/post.frag.spv

glslc defaultVertexColor.frag -o spv/defaultVertexColor.frag.spv
glslc defaultVertexColor.vert -o spv/defaultVertexColor.vert.spv
glslc defaultTexture.frag -o spv/defaultTexture.frag.spv
glslc defaultTexture.vert -o spv/defaultTexture.vert.spv
glslc defaultShadow.frag -o spv/defaultShadow.frag.spv
glslc defaultShadow.vert -o spv/defaultShadow.vert.spv

glslc terrain.frag -o spv/terrain.frag.spv
glslc terrain.vert -o spv/terrain.vert.spv
glslc terrainWire.frag -o spv/terrainWire.frag.spv
glslc terrainWire.vert -o spv/terrainWire.vert.spv
glslc terrainShadow.frag -o spv/terrainShadow.frag.spv
glslc terrainShadow.vert -o spv/terrainShadow.vert.spv

glslc sky.frag -o spv/sky.frag.spv
glslc sky.vert -o spv/sky.vert.spv