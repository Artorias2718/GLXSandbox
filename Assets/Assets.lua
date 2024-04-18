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
        { path = "Solid/Vertex.shader", arguments = { "vertex" } },
        { path = "Solid/Fragment.shader", arguments = { "fragment" } },
    }
}