return
{
    gameobjects =
    {
        "GameObjects/FlyCamera.lua"
    },
    materials = 
    {
        "Materials/Blue.lua",
        "Materials/Green.lua",
        "Materials/Red.lua",
        "Materials/Yellow.lua"
    },
    meshes =
    {
        "Meshes/Cube.lua",
        "Meshes/Icosphere.lua",
        "Meshes/Plane.lua",
        "Meshes/Suzanne.lua"
    },
    shaders =
    {
        { path = "Shaders/Solid/Vertex.eShader", arguments = { "vertex" } },
        { path = "Shaders/Solid/Fragment.eShader", arguments = { "fragment" } },
    }
}