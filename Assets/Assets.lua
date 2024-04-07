return
{
    gameobjects = 
    {
        "FlyCamera.lua"
    },
    meshes =
    {
        "Cube.lua",
        "Icosphere.lua",
        "Plane.lua",
        "Suzanne.lua"
    },
    shaders =
    {
        { path = "Vertex.shader", arguments = { "vertex" } },
        { path = "Fragment.shader", arguments = { "fragment" } },
    }
}