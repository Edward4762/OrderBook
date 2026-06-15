#pragma once
#include "Types.h"
#include "RingBuffer.h"

class OrderBook
{
    private: 
        RingBuffer<BufferData, 64> bids[120];
        RingBuffer<BufferData, 64> asks[120];
        int maxBid = 90;
        int minAsk = 120;
        LookupEntry orders[100001];

        int getMinAsk();
        void orderMatching();

    public:
        int getMaxBid();
        void addOrder(Order order);
        void CancelOrder(int id);
        void printOrderBook();
};
