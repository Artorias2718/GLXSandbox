return
{
    gameobjects =
    {
        "GameObjects/FlyCamera.lua"
    },
    materials =
    {
        "Materials/Boxes.lua",
        "Materials/Caboose.lua",
        "Materials/Cement.lua",
        "Materials/Sarge.lua"
    },
    meshes =
    {
        "Meshes/Boxes.lua",
        "Meshes/Caboose.lua",
        "Meshes/Cement.lua",
        "Meshes/Sarge.lua"
    },
    shaders =
    {
        { path = "Shaders/Solid/Vertex.eShader", arguments = { "vertex" } },
        { path = "Shaders/Solid/Fragment.eShader", arguments = { "fragment" } },
    }
}