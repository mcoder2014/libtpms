#ifndef IMPLICITFUNCTION_H
#define IMPLICITFUNCTION_H

#include <functional>
#include <vector>

using std::function;
using std::bind;

namespace ImplicitFunciton {

/**
 * @brief The OperationType enum
 * 隐函数结合的方法
 */
enum OperationType
{
    // 默认状态，即没有手动指定
    DEFAULT,
    // 相加:旧采样点的值与新的隐函数值相加
    ADD,
    // 与:旧采样点与新采样点，两者同时位于外部，则位于外部 DOUBLE MAX 否则 DOUBLE LOWEST
    AND,
    // 或:旧采样点与新采样点，两者任一位于外部，则位于外部
    OR,
    // 非:旧采样点的值取反
    NOT,
    // 异或:旧采样点与新采样点，取异或
    XOR
};

/**
 * @brief The ImplicitFunction class
 * 隐函数
 * std::function 支持如下的函数类型：
 * 1. 普通函数
 * 2. lambda 表达式
 * 3. 仿函数
 * 4. 类成员函数
 * 5. 类静态函数
 */
class ImplicitFunction
{
public:
    // 默认构造函数
    ImplicitFunction();

    /// 描述一个隐函数使用的数据
    // 隐函数
    std::function<double(double, double, double)> implicitFunction;

    // 权重函数
    std::function<double(double, double, double)> weightFunction;

    // 操作
    OperationType operation;
};

/**
 * @brief The ImplicitFunctionGroup class
 * 隐式函数组，多个隐式函数组成
 */
using ImplicitFuncitonGroup = std::vector<ImplicitFunction>;

}
#endif // IMPLICITFUNCTION_H
