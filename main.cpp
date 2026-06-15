#include <iostream>
#include <vector>
#include <thread>
#include <windows.h>
#include "Timer.h"
#include "Types.h"
#include "RingBuffer.h"
#include "OrderBook.h"

using namespace std;
atomic<bool> done{false};

void Producer(RingBuffer<OrderQueue, (1 << 16)> &SPSCQueue, int N)
{
    srand(time(0));
    int orderId = 1;
    for (int i = 0; i < N; i++)
    {
        Order newOrder = {orderId, 100, 1, (bool)(i % 2 == 0)};
        SPSCQueue.pushBack({newOrder, 1});
        if (i > 10 && i % 10 == 0)
        {
            Order cancelOrder = {orderId - 5, 0, 0, 0};
            SPSCQueue.pushBack({cancelOrder, 0});
        }
        orderId++;
    }
    done.store(true, memory_order_release);
}

void Consumer(RingBuffer<OrderQueue, (1 << 16)> &SPSCQueue, OrderBook *ob)
{
    long long queueTime = 0, addOrderTime = 0, totalTime = 0;
    int count = 0;

    while (!done.load(memory_order_acquire) || !SPSCQueue.isEmpty())
    {
        if (SPSCQueue.isEmpty())
            continue;

        Timer t1;
        Timer t2;
        t1.startTimer();
        t2.startTimer();
        OrderQueue tempOd = SPSCQueue.getfront();
        SPSCQueue.popFront();
        t2.endTimer();
        queueTime += t2.getTime();
        if (tempOd.isAdd)
        {
            t2.startTimer();
            ob->addOrder(tempOd.order);
            t2.endTimer();
            addOrderTime += t2.getTime();
        }
        else
        {
            ob->CancelOrder(tempOd.order.id);
        }

        t1.endTimer();
        totalTime += t1.getTime();
        count++;
    }

    cout << "Avg queue pop: " << queueTime / count << " ns\n";
    cout << "Avg addOrder: " << addOrderTime / count << " ns\n";
    cout << "Avg total per iter: " << totalTime / count << " ns\n";
}
int main()
{
    int N = 100000;
    OrderBook *ob = new OrderBook();

    RingBuffer<OrderQueue, (1 << 16)> SPSCQueue;

    Timer timer;
    timer.startTimer();
    thread t1(Producer, ref(SPSCQueue), N);
    thread t2(Consumer, ref(SPSCQueue), ob);
    SetThreadAffinityMask((HANDLE)t1.native_handle(), 1);
    SetThreadAffinityMask((HANDLE)t2.native_handle(), 2);
    t1.join();
    t2.join();
    timer.endTimer();

    long long total = timer.getTime();
    cout << "Total: " << total << " ns\n";
    cout << "Per order: " << total / N << " ns\n";
}