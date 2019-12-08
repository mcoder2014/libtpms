#ifndef ISOLEVELGENERATOR_H
#define ISOLEVELGENERATOR_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <string>
#include <QString>

#include <memory>

class IsoLevelGenerator
{
public:
    IsoLevelGenerator();

    cv::Mat load(const QString& filepath);
    cv::Mat load(const std::string& filepath);
    // input cvMat output vector<vector<float>>
};

#endif // ISOVALUEGENERATOR_H
