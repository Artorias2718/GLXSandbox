return
{
    gameobjects =
    {
        "GameObjects/FlyCamera.lua"
    },
    meshes =
    {
        "Meshes/Cube.lua",
        "Meshes/Sphere.lua",
        "Meshes/Plane.lua",
    },
    shaders =
    {
        { path = "Shaders/Vertex.eShader", arguments = { "vertex" } },
        { path = "Shaders/Fragment.eShader", arguments = { "fragment" } },
    }
}