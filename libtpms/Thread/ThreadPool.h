#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <atomic>
#include <future>
//#include <condition_variable>
//#include <thread>
//#include <functional>
#include <stdexcept>

// 线程池最大容量,应尽量设小一点
#define  THREADPOOL_MAX_NUM 32
//#define  THREADPOOL_AUTO_GROW

using std::function;
using std::thread;
using std::future;
using std::runtime_error;
using std::make_shared;
using std::packaged_task;
using std::forward;
using std::lock_guard;
using std::mutex;
using std::unique_lock;
using std::vector;
using std::queue;
using std::condition_variable;
using std::atomic;

/**
 * @brief The threadpool class
 * 线程池,可以提交变参函数或拉姆达表达式的匿名函数执行,可以获取执行返回值
 * 不直接支持类成员函数, 支持类静态成员函数或全局函数,Opteron()函数等
 */
class ThreadPool
{
    // 线程池中任务的定义
    using Task = function<void()>;

public:

    inline ThreadPool(unsigned short size = 4) { addThread(size); }

    inline ~ThreadPool()
    {
        running=false;

        // 唤醒所有线程执行
        taskCondition.notify_all();
        for (thread& thread : threadPools) {
            // 让线程“自生自灭”
            //thread.detach();
            if(thread.joinable()) {
                // 等待任务结束， 前提：线程一定会执行完
                thread.join();
            }
        }
    }

    /**
     * @brief commit
     * 提交一个任务
     * 调用.get()获取返回值会等待任务执行完,获取返回值
     * 有两种方法可以实现调用类成员，
     * 一种是使用   bind： .commit(std::bind(&Dog::sayHello, &dog));
     * 一种是用   mem_fn： .commit(std::mem_fn(&Dog::sayHello), this)
     * @param f
     * @param args
     * @return
     */
    template<class F, class... Args>
    auto commit(F&& f, Args&&... args) ->future<decltype(f(args...))>
    {
        // 线程池正在析构
        if (!running) throw runtime_error("commit on ThreadPool is stopped.");

        // typename std::result_of<F(Args...)>::type, 函数 f 的返回值类型
        using RetType = decltype(f(args...));

        // 把函数入口及参数,打包(绑定)
        auto task = make_shared<packaged_task<RetType()>>(bind(forward<F>(f), forward<Args>(args)...));

        future<RetType> future = task->get_future();
        {
            // 添加任务到队列
            // 对当前块的语句加锁  lock_guard 是 mutex 的 stack 封装类，
            // 构造的时候 lock()，析构的时候 unlock()
            lock_guard<mutex> lock{ mLock };

            // push(Task{...}) 放到队列后面
            taskQueue.emplace([task](){(*task)();});
        }
#ifdef THREADPOOL_AUTO_GROW
        if (_idlThrNum < 1 && _pool.size() < THREADPOOL_MAX_NUM)
            addThread(1);
#endif // !THREADPOOL_AUTO_GROW

        // 唤醒一个线程执行
        taskCondition.notify_one();

        return future;
    }

    // 空闲线程数量
    int idlCount() { return idlThrNum; }

    // 线程数量
    int thrCount() { return threadPools.size(); }

#ifndef THREADPOOL_AUTO_GROW
private:
#endif // !THREADPOOL_AUTO_GROW

    // 添加指定数量的线程
    void addThread(unsigned short size)
    {
        // 线程池中线程的任务
        auto threadTask = [this]{
            while (running)
            {
                // 获取一个待执行的 task
                Task task;
                {
                    // unique_lock 相比 lock_guard 的好处是：
                    // 可以随时 unlock() 和 lock()
                    unique_lock<mutex> lock{ mLock };

                    // wait 直到有 task
                    taskCondition.wait(lock, [this]{
                        return !running || !taskQueue.empty();
                    });

                    if (!running && taskQueue.empty()) return;

                    // 按先进先出从队列取一个 task
                    task = move(taskQueue.front());
                    taskQueue.pop();
                }

                idlThrNum--;
                // 执行任务
                task();
                idlThrNum++;
            }
        };
        for (unsigned short i = 0; threadPools.size() < THREADPOOL_MAX_NUM && i < size; i++) {
            // 增加线程数量,但不超过 预定义数量 THREADPOOL_MAX_NUM
            threadPools.emplace_back(threadTask);
            idlThrNum++;
        }
    }

private:
    // 线程池数据
    vector<thread> threadPools;

    // 任务队列
    queue<Task> taskQueue;

    // 同步
    mutex mLock;

    // 条件阻塞
    condition_variable taskCondition;

    // 线程池是否执行
    atomic<bool> running{ true };

    // 空闲线程数量
    atomic<int>  idlThrNum{ 0 };
};

#endif  //https://github.com/lzpong/
