#pragma once
#include "Types.h"
#include "OrderBook.h"
#include<unordered_map>
#include<string>

class OrderBookManager{
    OrderBook* books[20000] = {nullptr};
    unordered_map<string,int> symbolToId;
    int nextId = 0;

    public:
        void addOrder(OrderSymbol orderWithSymbol);
        int getId(string symbol);
};