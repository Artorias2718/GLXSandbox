return
{
    gameobjects =
    {
        "GameObjects/FlyCamera.lua"
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