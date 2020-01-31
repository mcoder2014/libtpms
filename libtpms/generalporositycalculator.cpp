#include "generalporositycalculator.h"

#include <eigen3/Eigen/Dense>

GeneralPorosityCalculator::GeneralPorosityCalculator()
{

}

GeneralPorosityCalculator::~GeneralPorosityCalculator()
{

}


///
/// \brief GeneralPorosityCalculator::getPorosity
/// 通用方法
/// \param porousStructure
/// \param boundary
/// \param voxelSize
/// \return
///
float GeneralPorosityCalculator::getPorosity(
        SurfaceMesh::SurfaceMeshModel &porousStructure,
        SurfaceMesh::SurfaceMeshModel &boundary,
        float voxelSize)
{
    porousStructure.updateBoundingBox();
    boundary.updateBoundingBox();
    Octree oct_boundary(&boundary);
    Octree oct_porous(&porousStructure);

    uint count_one = 0;
    uint count_zero = 0;

    Eigen::AlignedBox3d bbox = boundary.bbox();
    // 根据包围盒去设计

    Eigen::Vector3d relative_size = bbox.max()-bbox.min();
    Eigen::Vector3i sample_size(
        std::lround(relative_size[0]/voxelSize),
        std::lround(relative_size[1]/voxelSize),
        std::lround(relative_size[2]/voxelSize));
    Eigen::Vector3d range_min = bbox.min();
    Eigen::Vector3d range_max = bbox.max();

    float lower_bwlow_range = range_min[2] - 100;
    /// 从一面发出射线和 boundary 模型相交，确定哪些采样点属于模型内部
    std::vector<std::vector<Eigen::Vector3d>> points_mat(
                sample_size[0], std::vector<Eigen::Vector3d>(
                    sample_size[1],Eigen::Vector3d(0.0,0.0,lower_bwlow_range)));

    // 初始化一个发送射线的平面二维矩阵
    // 矩阵密度均匀，位置在包围盒 z 轴下部
    for(int iter_x = 0; iter_x < sample_size[0]; iter_x++)
    {
        int co_x = range_min[0] + iter_x * voxelSize;
        for(int iter_y = 0; iter_y < sample_size[1]; iter_y++)
        {
            int co_y = range_min[1] + iter_y * voxelSize;

            points_mat[iter_x][iter_y][0] = co_x;
            points_mat[iter_x][iter_y][1] = co_y;
        }
    }

    // 为了方便计算点的个数，初始化一个 Z 轴向量
    std::vector<float> points_z(sample_size[2]);
    for(int i = 0; i < sample_size[2]; i++)
    {
        points_z[i] = range_min[2] + i * voxelSize;
    }

    /// 最后通过计算两个点数，计算孔隙率
    int voxelBoundary = calcInner(points_mat, points_z, voxelSize, oct_boundary);
    int voxelPorous = calcInner(points_mat, points_z, voxelSize, oct_porous);
    float porosity = 1.0 * voxelPorous / voxelBoundary;

    std::cout << "Total Points: " << sample_size[0] * sample_size[1] * sample_size[2]
             << "\nPoints in Boundary: " << voxelBoundary
             << "\nPoints in Porous: " << voxelPorous
             << "\nPorosity: " << porosity;

    /// 返回孔隙率的结果
    return porosity;

}

float GeneralPorosityCalculator::getPorosity(
        SurfaceMesh::SurfaceMeshModel &porousStructure,
        float voxelSize)
{
    std::cout << "Get Pososity" << std::endl;
    porousStructure.updateBoundingBox();
    Octree oct_porous(&porousStructure);
    Eigen::AlignedBox3d bbox = porousStructure.bbox();
    // 根据包围盒去设计

    Eigen::Vector3d relative_size = bbox.max()-bbox.min();
    Eigen::Vector3i sample_size(
        std::lround(relative_size[0]/voxelSize),
        std::lround(relative_size[1]/voxelSize),
        std::lround(relative_size[2]/voxelSize));
    Eigen::Vector3d range_min = bbox.min();
    Eigen::Vector3d range_max = bbox.max();

    std::cout << "Init sample box" << std::endl;
    float lower_bwlow_range = range_min[2] - 100;
    /// 从一面发出射线和 boundary 模型相交，确定哪些采样点属于模型内部
    std::vector<std::vector<Eigen::Vector3d>> points_mat(
                sample_size[0], std::vector<Eigen::Vector3d>(
                    sample_size[1],Eigen::Vector3d(0.0,0.0,lower_bwlow_range)));

    // 初始化一个发送射线的平面二维矩阵
    // 矩阵密度均匀，位置在包围盒 z 轴下部
    for(int iter_x = 0; iter_x < sample_size[0]; iter_x++)
    {
        int co_x = range_min[0] + iter_x * voxelSize;
        for(int iter_y = 0; iter_y < sample_size[1]; iter_y++)
        {
            int co_y = range_min[1] + iter_y * voxelSize;

            points_mat[iter_x][iter_y][0] = co_x;
            points_mat[iter_x][iter_y][1] = co_y;
        }
    }

    // 为了方便计算点的个数，初始化一个 Z 轴向量
    std::vector<float> points_z(sample_size[2]);
    for(int i = 0; i < sample_size[2]; i++)
    {
        points_z[i] = range_min[2] + i * voxelSize;
    }

    std::cout << "Calc inner" << std::endl;
    /// 最后通过计算两个点数，计算孔隙率
    int voxelBoundary = calcInner(points_mat, points_z, bbox);
    int voxelPorous = calcInner(points_mat, points_z, voxelSize, oct_porous);
    float porosity = 1.0 * voxelPorous / voxelBoundary;

    std::cout << "Total Points: " << sample_size[0] * sample_size[1] * sample_size[2]
             << "\nPoints in Boundary: " << voxelBoundary
             << "\nPoints in Porous: " << voxelPorous
             << "\nPorosity: " << porosity;

    /// 返回孔隙率的结果
    return porosity;
}

int GeneralPorosityCalculator::calcInner(
        std::vector<std::vector<Eigen::Vector3d>>& points_mat,
        std::vector<float> &helper_z,
        float voxelSize,
        Octree& oct_model)
{
    Eigen::Vector3i sample_size(
        points_mat.size(),
        points_mat[0].size(),
            helper_z.size());

    float range_down = helper_z[0];

    int count = 0;

    Eigen::Vector3d up_direction(0,0,1);
    /// 再发出射线和 porous 模型相交，确定哪些段是内部和外部
    for(int iter_x = 0; iter_x < sample_size[0]; iter_x++)
    {
        for(int iter_y = 0; iter_y < sample_size[1]; iter_y++)
        {
            // 发射射线
            Ray up_ray(points_mat[iter_x][iter_y], up_direction);
            QSet<int> down_intersects = oct_model.intersectRay(
                        up_ray, 0.000001, false);

            HitResult res;
            std::vector<float> intersect_z;

            for(int face_index : down_intersects)
            {
                oct_model.rayTriangleIntersectionTest(
                    SurfaceMesh::Model::Face(face_index), up_ray, res, false);

                if(res.hit)
                {
                    // 统计交点
                    Eigen::Vector3d intersect_point = up_ray.origin + up_ray.direction * res.distance;
                    intersect_z.push_back(intersect_point[2]);
                }
            }

            // 分析哪部分位于内部哪部分位于外部
            uint intersect_count = intersect_z.size();
            if(intersect_count %2 != 0)
            {
                // 理论上只有偶数个交点，因此如果有奇数个交点是错误情况
                // 暂时跳过不计入总数

                std::cout << "There are " << intersect_count << " intersection points"
                          << " at [" << iter_x << "," << iter_y << "]"
                          << " ERROR" << std::endl;
                continue;
            }

            // 两两一组，快速计算范围内点的数量
            std::sort(intersect_z.begin(), intersect_z.end());
            for(int i=0; i < intersect_z.size()-1 && intersect_count > 0; i+=2)
            {
                float lower = intersect_z[i];
                float higher = intersect_z[i+1];

                int tmp_count = static_cast<int>(
                    (higher - range_down)/voxelSize - (lower-range_down)/voxelSize);
                count += tmp_count;
            }

        }   // for(int iter_y = 0; iter_y < sample_size[1]; iter_y++)
    }   // for(int iter_x = 0; iter_x < sample_size[0]; iter_x++)

    return count;
}

int GeneralPorosityCalculator::calcInner(
        std::vector<std::vector<Eigen::Vector3d> > &points_mat,
        std::vector<float> &helper_z,
        Eigen::AlignedBox3d boundary)
{
    Eigen::Vector3i sample_size(
        points_mat.size(),
        points_mat[0].size(),
        helper_z.size());

    int count = 0;

    for(int iter_x = 0; iter_x < sample_size[0]; iter_x++)
    {
        for(int iter_y = 0; iter_y < sample_size[1]; iter_y++)
        {
            // 从数组中找到两头刚好在包围盒内的点的数量
            Eigen::Vector3d pos = points_mat[iter_x][iter_y];

            // 找上界
            int top = helper_z.size()-1;
            for(; top >=0; top--)
            {
                pos[2] = helper_z[top];
                if(boundary.contains(pos))
                {
                    // 包含
                    break;
                }
            }

            // 找下界
            int down = 0;
            for(; down < top; down++)
            {
                pos[2] = helper_z[down];
                if(boundary.contains(pos))
                {
                    // 包含
                    break;
                }
            }
            count += (top - down);
        }   // for(int iter_y = 0; iter_y < sample_size[1]; iter_y++)
    }   // for(int iter_x = 0; iter_x < sample_size[0]; iter_x++)

    return count;
}
