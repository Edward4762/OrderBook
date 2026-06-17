#include "OrderBook.h"
#include "Timer.h"
#include<iostream>

int OrderBook::getMinAsk(){
    while (minAsk < 120 && asks[minAsk].isEmpty()){
        minAsk++;
    }
    return minAsk;
}

void OrderBook::orderMatching(){
    while (minAsk <= maxBid){
        while (!bids[maxBid].isEmpty() && !asks[minAsk].isEmpty()){
            BufferData bestBid = bids[maxBid].getfront();
            BufferData bestAsk = asks[minAsk].getfront();
            int bidVolume = bestBid.volume;
            int askVolume = bestAsk.volume;
            bids[maxBid].popFront();
            asks[minAsk].popFront();
            LookupEntry bestBidEntry = orders[bestBid.id];
            LookupEntry bestAskEntry = orders[bestAsk.id];
            orders[bestBid.id] = {0, 0, 0};
            orders[bestAsk.id] = {0, 0, 0};
            if (askVolume > bidVolume){
                asks[minAsk].pushFront({askVolume - bidVolume, bestAsk.id});
                orders[bestAsk.id] = bestAskEntry;
            }
            else if (askVolume < bidVolume){
                bids[maxBid].pushFront({bidVolume - askVolume, bestBid.id});
                orders[bestBid.id] = bestBidEntry;
            }
        }
        maxBid = getMaxBid();
        minAsk = getMinAsk();
    }
    return;
}

int OrderBook::getMaxBid(){
    while (maxBid > 90 && bids[maxBid].isEmpty()){
        maxBid--;
    }
    return maxBid;
}

void OrderBook::addOrder(Order order){
    if (order.isBuy){
        int buyPrice = order.price;
        int buyQuantity = order.quantity;
        int id = order.id;
        BufferData tempData = {buyQuantity, id};
        int bufferIndex = bids[buyPrice].pushBack(tempData);
        orders[order.id] = {buyPrice, bufferIndex, 1};
        maxBid = std::max(maxBid, buyPrice);
    }
    else{
        int askPrice = order.price;
        int askQuantity = order.quantity;
        int id = order.id;
        BufferData tempData = {askQuantity, id};
        int bufferIndex = asks[askPrice].pushBack(tempData);
        orders[order.id] = {askPrice, bufferIndex, 0};
        minAsk = std::min(minAsk, askPrice);
    }
    orderMatching();
}

void OrderBook::CancelOrder(int id){
    if (orders[id].price == 0){
        return;
    }
    int price = orders[id].price;
    int index = orders[id].index;
    if (orders[id].isBuy){
        bids[price].cancelOrder(index);
    }
    else{
        asks[price].cancelOrder(index);
    }
    orders[id] = {0, 0, 0};
}

void OrderBook::printOrderBook(){
    cout<<"--------------------- Bids -------------------- "<<'\n';
    int bestBid = getMaxBid();
    for(int p = bestBid;p>=90;p--){
        int accumulatedVolume = 0;
        int st = bids[p].getFront();
        int en = bids[p].getEnd();
        while(st!=en){
            accumulatedVolume += (bids[p].getElement(st)).volume;
            st = (st + 1)%64;
        }

        if(accumulatedVolume > 0){
            cout<<"BID PRICE : "<<p<<" "<<"Volume : "<<accumulatedVolume<<'\n';
        }
    }
    cout<<"--------------------- Asks -------------------- "<<'\n';
    int bestAsk = getMinAsk();
    for(int p = bestAsk;p<=120;p++){
        int accumulatedVolume = 0;
        int st = asks[p].getFront();
        int en = asks[p].getEnd();
        while(st!=en){
            accumulatedVolume += (asks[p].getElement(st)).volume;
            st = (st + 1)%64;
        }
        if(accumulatedVolume > 0){
            cout<<"ASK PRICE : "<<p<<" "<<"Volume : "<<accumulatedVolume<<'\n';
        }
    }
}