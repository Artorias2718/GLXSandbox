return
{
    gameobjects =
    {
        "GameObjects/FlyCamera.lua"
    },
    meshes =
    {
        "Meshes/Cube.lua",
        "Meshes/Plane.lua"
    },
    shaders =
    {
        { path = "Shaders/Vertex.eShader", arguments = { "vertex" } },
        { path = "Shaders/Fragment.eShader", arguments = { "fragment" } },
    }
}