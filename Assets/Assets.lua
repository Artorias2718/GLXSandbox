return
{
    meshes =
    {
        "Meshes/Square.lua",
        "Meshes/Triangle.lua"
    },
    shaders =
    {
        { path = "Shaders/Vertex.eShader", arguments = { "vertex" } },
        { path = "Shaders/Fragment.eShader", arguments = { "fragment" } },
    }
}