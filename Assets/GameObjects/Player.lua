return
{
    name = "Player",
    active = false,
    height = 50.0,
    width = 5.0,
    transform =
    {
        position = { x = 0, y = 3.5, z = 0 },
        orientation = { angle = 0, axis = { x = 0, y = 0, z = 0 } },
        scale = { x = 1.0, y = 1.0, z = 1.0 }
    },
    rigidbody =
    {
        mass = 10,
        applyGravity = true,
        gravity = { x = 0, y = -1.81, z = 0 },
        speed = 1 
    }
}