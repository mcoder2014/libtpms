#ifndef BASETPMSALGORITHM_H
#define BASETPMSALGORITHM_H

#include <memory>

#include "BaseTpmsConfig.h"


class BaseTpmsAlgorithm
{
public:
    virtual ~BaseTpmsAlgorithm(){};
    /**
     * 执行算法
     * @brief process
     */
    virtual void process() = 0;

    /**
     * 设置 配置
     * @brief setConfig
     * @param config
     */
    virtual void setConfig(std::shared_ptr<BaseTpmsConfig> config) = 0;
};

#endif // BASETPMSALGORITHM_H
