#ifndef BOXTPMSSINGESURFACEALGORITHM_H
#define BOXTPMSSINGESURFACEALGORITHM_H

#include <memory>
#include <vector>

#include "BaseTpmsAlgorithm.h"
#include "BoxTpmsSingleSurfaceConfig.h"
#include "SamplePoint.h"

using std::vector;

class BoxTpmsSingeSurfaceAlgorithm
        :public BaseTpmsAlgorithm
{
public:
    BoxTpmsSingeSurfaceAlgorithm();
    virtual Mesh process() override;
    virtual void setConfig(std::shared_ptr<BaseTpmsConfig> config) override {
        this->config = std::dynamic_pointer_cast<BoxTpmsSingleSurfaceConfig>(config);
    }
    void setConfig(std::shared_ptr<BoxTpmsSingleSurfaceConfig> config) {
        this->config = config;
    }
    std::shared_ptr<BoxTpmsSingleSurfaceConfig> getConfig() {
        return config;
    }

protected:
    void init();
    void clear();
    Mesh marchMesh(vector<vector<vector<SamplePoint> > > &sampleMatrix);
    void addFaces(int *faces, Vector3i index, Mesh& mesh);
    Mesh::VertexHandle getVertexHandle(Vector3i index, int edgeIndex, Mesh &mesh);

    std::shared_ptr<BoxTpmsSingleSurfaceConfig> config;
    vector<vector<vector<SamplePoint> > > sampleMatrix;
    std::unordered_map<std::string, int> vertexIdMap;

};

#endif // BOXTPMSSINGESURFACEALGORITHM_H
