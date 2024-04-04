return
{
    gameobjects = 
    {
        "FlyCamera.lua"
    },
    meshes =
    {
        "Cube.lua",
        "Plane.lua"
    },
    shaders =
    {
        { path = "Vertex.shader", arguments = { "vertex" } },
        { path = "Fragment.shader", arguments = { "fragment" } },
    }
}