return
{
    gameobjects =
    {
        "GameObjects/FlyCamera.lua"
    },
    meshes =
    {
        "Meshes/Boxes.lua",
        "Meshes/Caboose.lua",
        "Meshes/Sarge.lua"
    },
    shaders =
    {
        { path = "Shaders/Solid/Vertex.eShader", arguments = { "vertex" } },
        { path = "Shaders/Solid/Fragment.eShader", arguments = { "fragment" } },
    }
}