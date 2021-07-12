return
{
    gameobjects =
    {
        "GameObjects/FlyCamera.lua"
    },
    meshes =
    {
        "Meshes/Caboose.lua",
        "Meshes/Sarge.lua"
    },
    shaders =
    {
        { path = "Shaders/Vertex.eShader", arguments = { "vertex" } },
        { path = "Shaders/Fragment.eShader", arguments = { "fragment" } },
    }
}