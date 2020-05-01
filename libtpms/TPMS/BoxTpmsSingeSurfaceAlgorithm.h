#ifndef BOXTPMSSINGESURFACEALGORITHM_H
#define BOXTPMSSINGESURFACEALGORITHM_H

#include<memory>

#include "BaseTpmsAlgorithm.h"
#include "BoxTpmsSingleSurfaceConfig.h"

class BoxTpmsSingeSurfaceAlgorithm
        :public BaseTpmsAlgorithm
{
public:
    BoxTpmsSingeSurfaceAlgorithm();
    virtual Mesh process() override;
    virtual void setConfig(std::shared_ptr<BaseTpmsConfig> config) override {
        this->config = std::dynamic_pointer_cast<BoxTpmsSingleSurfaceConfig>(config);
    }
    std::shared_ptr<BoxTpmsSingleSurfaceConfig> getConfig() {
        return config;
    }

private:
    std::shared_ptr<BoxTpmsSingleSurfaceConfig> config;
};

#endif // BOXTPMSSINGESURFACEALGORITHM_H
