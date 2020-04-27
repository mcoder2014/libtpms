#ifndef BOXTPMSSINGESURFACEALGORITHM_H
#define BOXTPMSSINGESURFACEALGORITHM_H

#include<memory>

#include "BaseTpmsAlgorithm.h"

class BoxTpmsSingeSurfaceAlgorithm
        :public BaseTpmsAlgorithm
{
public:
    BoxTpmsSingeSurfaceAlgorithm();
    virtual void process() override;
    virtual void setConfig(std::shared_ptr<BaseTpmsConfig> config) override {
        this->config = std::dynamic_pointer_cast<BoxTpmsSingeSurfaceAlgorithm>(config);
    }
    std::shared_ptr<BoxTpmsSingeSurfaceAlgorithm> getConfig() {
        return config;
    }

private:
    std::shared_ptr<BoxTpmsSingeSurfaceAlgorithm> config;
};

#endif // BOXTPMSSINGESURFACEALGORITHM_H
