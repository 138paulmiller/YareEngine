
## TODO
- Replace all glm and gl calls. Only include them once in math and graphics modules
- Eventually compile all Yare/[Modules] to be linked with Yare 
- System
	Serializable Base class
	Create Archiver class that uses FileSystem and serializable for config/save/asset/meta data

- Graphics
	Renderable Base class
		- With a render proxy 
			- Proxy is destroyed when Renderable is
			- Is instantied and used on render thread only!
    Viewport 
        Multiple Renderable windows 

    - Scene
        - Octree of Components
        - Sort Renderables by their shader id.
        - Shadermanager is what allocs, deallocates, binds and unbinds shaders.
            - All renderables must have a shader id
        - Has a single camera
        - Has a octree of lights
    - Pass
        - Contains multiple scenes that each render to a specified target textures
        - 