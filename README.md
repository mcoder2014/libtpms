# Implicit Surface

This project is a test project about how to create a (Triply Periodic Minimal Surfaces, TPMS) porosus structure model and how to convert it into a traditional triangle mesh. 

This project uses the **basic marching box algorithm**, which can easily convert an implicit surface into traingle mesh with low precision.

This project can alse convert some fixed triangle mesh into TPMS porosus structure, will show below.

**Welcome stars!**

## Support Implicit Surface Types

This project create some type of Triply Periodic Minimal Surfaces, such as:

- P type
- D
- G
- I-WP
- F-RD
- L
- Tubular P
- Tubular G

# Example

```cpp
Mesh* createCubeTPMS(
        string type, Eigen::Vector3i sample, Eigen::Vector3i density,
        Eigen::Vector3d rangeMin, Eigen::Vector3d rangeMax,
        float isoLevel, float depth, int smooth_times = 10)
{
    ImplicitSurface implicit_surface;
    MarchBox march_box;
    SmoothTool  smoothTool;

    // Setting
    implicit_surface.setType(type);

    march_box.setRange(rangeMin, rangeMax);
    march_box.setDensity(density);
    march_box.setSampleSize(sample);
    march_box.setReverse(false);

    // 1. marchbox
    march_box.marching_cubes(implicit_surface, isoLevel);

    // 2. smooth
    smoothTool.createMesh(march_box.m_vertices, march_box.m_faces);
    smoothTool.basicSmooth(smooth_times);

    // 3. offset
    Mesh *result = new Mesh();
    result->assign(*(smoothTool.getMesh()));
    result = smoothTool.meshOffset(result, depth);

    return result;
}
```

You can create many different type of TMPS porous structure using the code like the example upon.
Some result of the code is screensnap at here.

![](imgs/types.png)

The below is to use a insole model to create a TPMS porosus model.

![](imgs/insole.png)


# Environment
1. greater than Qt 5
2. glm library
3. [Starlab](https://github.com/OpenGP/starlab)
4. openmesh 8.0

# Reference
1. [Marching Cubes](http://paulbourke.net/geometry/polygonise/)
2. [Computer-aided porous scaffold design for tissue engineering using triply periodic minimal surfaces](https://link.springer.com/article/10.1007/s12541-011-0008-9)