#include "OrderBookManager.h"
#include<iostream>

void OrderBookManager::addOrder(OrderSymbol orderWithSymbol){
    string Symbol = orderWithSymbol.Symbol;
    int id = getId(Symbol); 
    if(orderWithSymbol.isAdd){
        books[id]->addOrder(orderWithSymbol.order);
    }
    else{
        books[id]->CancelOrder(orderWithSymbol.order.id);
    }
}


int OrderBookManager::getId(string symbol){
    if(symbolToId.find(symbol)!=symbolToId.end()){
        return symbolToId[symbol];
    }
    symbolToId[symbol] = nextId;
    books[nextId] = new OrderBook();
    return nextId++;
}