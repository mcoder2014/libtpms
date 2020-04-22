#include "isolevelgenerator.h"

IsoLevelGenerator::IsoLevelGenerator()
{

}

cv::Mat IsoLevelGenerator::load(const std::string &filepath)
{
    printf("%s\n", filepath.data());
    return cv::Mat(10,10,CV_8UC3);
}
