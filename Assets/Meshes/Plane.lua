return
{
    vertices = 
    {
        -- Vertex 0
        {    
            position = { x = -1.0, y = -0.25, z = 1.0 },
            color = { r = 1.0, g = 1.0, b = 1.0, a = 1.0 },
        },
        -- Vertex 1 
        {
            position = { x = 1.0, y = -0.25, z = 1.0 },
            color = { r = 1.0, g = 1.0, b = 1.0, a = 1.0 },
        },
        -- Vertex 2
        {
            position = { x = 1.0, y = -0.25, z = -1.0 },
            color = { r = 1.0, g = 1.0, b = 1.0, a = 1.0 },
        },
        -- Vertex 3
        {
            position = { x = -1.0, y = -0.25, z = -1.0 },
            color = { r = 1.0, g = 1.0, b = 1.0, a = 1.0 },
        }
    },
    indices = 
    {
        -- Triangle 0
        { a = 0, b = 1, c = 2 },
        -- Triangle 1 
        { a = 0, b = 2, c = 3 }
    },
};