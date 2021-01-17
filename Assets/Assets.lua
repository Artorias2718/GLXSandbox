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
        { path = "Shaders/Vertex.eShader", arguments = { "vertex" } },
        { path = "Shaders/Fragment.eShader", arguments = { "fragment" } },
    }
}