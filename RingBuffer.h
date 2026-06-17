#pragma once
#include <atomic>
using namespace std;

template <typename T, int SIZE>
class RingBuffer
{
    T value[SIZE];
    bool isCancelled[SIZE] = {};
    alignas(64) atomic<int> front{0};
    alignas(64) atomic<int> end{0};

    
    void incrementFront()
    {
        front.store((front.load(memory_order_acquire) + 1) % SIZE, memory_order_release);
    }
    void decrementFront()
    {
        front.store((front.load(memory_order_acquire) - 1 + SIZE) % SIZE, memory_order_release);
    }
    void incrementEnd()
    {
        end.store((end.load(memory_order_acquire) + 1) % SIZE, memory_order_release);
    }
    
    public:
    int getFront()
    {
        return front.load(memory_order_acquire);
    }

    int getEnd()
    {
        return end.load(memory_order_acquire);
    }
    
    bool isEmpty()
    {
        return getFront() == getEnd();
    }

    bool isFull()
    {
        return (getEnd() + 1) % SIZE == getFront();
    }

    T getfront()
    {
        int updFront = getFront();
        while (isCancelled[updFront])
        {
            isCancelled[updFront] = false;
            incrementFront();
        }
        updFront = getFront();
        return value[updFront];
    }

    T getElement(int index){
        return value[index];
    }

    void popFront()
    {
        incrementFront();
        return;
    }

    int pushFront(T data)
    {
        while (isFull())
            ;
        decrementFront();
        int updFront = getFront();
        value[updFront] = data;
        return updFront;
    }

    int pushBack(T data)
    {
        while (isFull())
            ;
        int updEnd = getEnd();
        value[updEnd] = data;
        incrementEnd();
        return updEnd;
    }

    void cancelOrder(int index)
    {
        if (!isCancelled[index])
        {
            isCancelled[index] = 1;
        }
    }
};