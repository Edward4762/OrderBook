#pragma once
#include<string>

struct Order
{
    int id;
    int price;
    int quantity;
    bool isBuy;
};

struct OrderSymbol{
    std::string Symbol;
    Order order;
    bool isAdd;
};

struct OrderQueue
{
    OrderSymbol order;
};

struct LookupEntry
{
    int price;
    int index;
    bool isBuy;
};

struct BufferData
{
    int volume;
    int id;
};