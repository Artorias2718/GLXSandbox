import bpy
import os

class ExportMeshDataOperator(bpy.types.Operator):
    """Export mesh data to Lua format"""
    bl_idname = "object.export_mesh_data"
    bl_label = "GLX (.lua)"

    filepath: bpy.props.StringProperty(subtype="FILE_PATH")
    dialog_open: bpy.props.BoolProperty(default=False)

    def execute(self, context):
        if not self.filepath:
            self.report({'ERROR'}, "No file selected")
            return {'CANCELLED'}

        # Get selected object
        obj = bpy.context.object
        mesh = obj.data
        
        # Get vertices and indices
        vertices = []
        vertex_indices = {}
        indices = []

        # Retrieve vertex colors
        if mesh.vertex_colors:
            vertex_colors = mesh.vertex_colors.active.data
        else:
            vertex_colors = None

        # Get world matrix
        world_matrix = obj.matrix_world

        for poly in mesh.polygons:
            if len(poly.vertices) == 4:  # Handle quads
                # First triangle
                for j in [0, 1, 2]:
                    loop_index = poly.loop_indices[j]
                    loop = mesh.loops[loop_index]
                    vertex_index = loop.vertex_index
                    
                    #Transform vertex position by object's transformation matrix
                    vertex_co = world_matrix @ mesh.vertices[vertex_index].co
                    
                    # Add transformed vertex to vertices list
                    if vertex_index not in vertex_indices:
                        if vertex_colors:
                            color = vertex_colors[loop_index].color
                        else:
                            color = (1.0, 0.0, 0.0, 1.0)  # Default color
                        vertices.append({'position': vertex_co, 'color': color})
                        vertex_indices[vertex_index] = len(vertices) - 1
                    indices.append(vertex_indices[vertex_index])

                # Second triangle
                for j in [0, 2, 3]:
                    loop_index = poly.loop_indices[j]
                    loop = mesh.loops[loop_index]
                    vertex_index = loop.vertex_index
                    
                    #Transform vertex position by object's transformation matrix
                    vertex_co = world_matrix @ mesh.vertices[vertex_index].co
                    
                    # Add transformed vertex to vertices list
                    if vertex_index not in vertex_indices:
                        if vertex_colors:
                            color = vertex_colors[loop_index].color
                        else:
                            color = (1.0, 0.0, 0.0, 1.0)  # Default color
                        vertices.append({'position': vertex_co, 'color': color})
                        vertex_indices[vertex_index] = len(vertices) - 1
                    indices.append(vertex_indices[vertex_index])
            else:  # Handle triangles
                for loop_index in poly.loop_indices:
                    loop = mesh.loops[loop_index]
                    vertex_index = loop.vertex_index
                    
                    #Transform vertex position by object's transformation matrix
                    vertex_co = world_matrix @ mesh.vertices[vertex_index].co
                    
                    # Add transformed vertex to vertices list
                    if vertex_index not in vertex_indices:
                        if vertex_colors:
                            color = vertex_colors[loop_index].color
                        else:
                            color = (1.0, 0.0, 0.0, 1.0)  # Default color
                        vertices.append({'position': vertex_co, 'color': color})
                        vertex_indices[vertex_index] = len(vertices) - 1
                    indices.append(vertex_indices[vertex_index])

        # Construct Lua-formatted string
        lua_str = "return {\n"
        lua_str += "    vertices = {\n"
        for i, vertex in enumerate(vertices):
            lua_str += f"        -- Vertex {i}\n"
            lua_str += "        {\n"
            lua_str += f"            position = {{ x = {str(vertex['position'].x)}, y = {str(vertex['position'].y)}, z = {str(vertex['position'].z)} }},\n"
            lua_str += f"            color = {{ r = {str(vertex['color'][0])}, g = {str(vertex['color'][1])}, b = {str(vertex['color'][2])}, a = {str(vertex['color'][3])} }},\n"
            lua_str += "        },\n"
        lua_str += "    },\n"

        lua_str += "    indices = {\n"
        for i in range(0, len(indices), 3):
            lua_str += f"        -- Triangle {i // 3}\n"
            lua_str += f"        {{ a = {str(indices[i])}, b = {str(indices[i + 1])}, c = {str(indices[i + 2])} }},\n"
        lua_str += "    },\n"

        lua_str += "};"

        # Save Lua-formatted string to a file
        try:
            with open(self.filepath, 'w') as file:
                file.write(lua_str)
            self.report({'INFO'}, "Mesh data exported to: " + self.filepath)
        except Exception as e:
            self.report({'ERROR'}, "An error occurred: " + str(e))
            return {'CANCELLED'}

        return {'FINISHED'}

    def invoke(self, context, event):
        if not self.dialog_open:
            context.window_manager.fileselect_add(self)
            self.dialog_open = True
        else:
            return {'CANCELLED'}

        return {'RUNNING_MODAL'}

class ExportMeshDataMenu(bpy.types.Menu):
    bl_idname = "OBJECT_MT_export_mesh_data"
    bl_label = "Export Mesh Data"

    def draw(self, context):
        layout = self.layout
        layout.operator(ExportMeshDataOperator.bl_idname)

    def invoke(self, context, event):
        if not self.dialog_open:
            context.window_manager.fileselect_add(self)
            self.dialog_open = True
        else:
            return {'CANCELLED'}

        return {'RUNNING_MODAL'}

def menu_func(self, context):
    self.layout.menu(ExportMeshDataMenu.bl_idname)

def register():
    bpy.utils.register_class(ExportMeshDataOperator)
    bpy.utils.register_class(ExportMeshDataMenu)
    bpy.types.TOPBAR_MT_file_export.append(menu_func)

def unregister():
    bpy.utils.unregister_class(ExportMeshDataOperator)
    bpy.utils.unregister_class(ExportMeshDataMenu)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()
