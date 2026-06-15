#pragma once

struct Order
{
    int id;
    int price;
    int quantity;
    bool isBuy;
};

struct OrderQueue
{
    Order order;
    bool isAdd;
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