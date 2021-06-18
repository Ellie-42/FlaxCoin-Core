#include "block.h"
#include <iostream>

block::block(unsigned int index, std::string prevHash, unsigned int timestamp, std::vector<std::string> data, int diff)
{
    std::cout<<"Index! !!! : " <<index<<std::endl;
    this->index = index;
    this->prevHash = prevHash;
    this->timestamp = timestamp;
    this->data = data;
    this->nonce = 0;
    this->diff = diff;
}

block::block(Json::Value jsonData)
{
    this->index = jsonData["index"].asUInt();
    this->prevHash = jsonData["prevHash"].asString();
    this->curHash = jsonData["curHash"].asString();
    this->timestamp = jsonData["timestamp"].asUInt();
    this->nonce = jsonData["nonce"].asInt();
    Json::Value valueData = jsonData["data"];
    this->diff = jsonData["diff"].asInt();
    for(int i = 1; i <= valueData[0].asInt(); i++)
    {
        this->data.push_back(valueData[i].asString());
    }
}

void block::addTxData(std::string txData)
{
    block::data.push_back(txData);
}

std::string block::combineTxData()
{
    std::string blockDat = "";
    for(std::string dat : data)
    {
        blockDat += dat;
    }
    return blockDat;
}

Json::Value block::toJson() const
{
    Json::Value value(Json::objectValue);
    value["index"] = Json::Value(this->index);
    value["prevHash"] = Json::Value(this->prevHash);
    value["curHash"] = Json::Value(this->curHash);
    value["timestamp"] = Json::Value(this->timestamp);
    value["nonce"] = Json::Value(this->nonce);
    value["diff"] = Json::Value(this->diff);
    Json::Value dataValue(Json::arrayValue);
    dataValue[0] = this->data.size();
    for(unsigned int i = 1; i <= this->data.size(); i++)
    {
        dataValue[i] = data.at(i-1);
    }
    value["data"] = dataValue;
    return value;
}

std::ostream& operator<< ( std::ostream& os , const block& bl)
{
    os << "Difficulty:   " << std::to_string(bl.diff) << "\nIndex:        "<< std::to_string(bl.index) << "\nCurrent Hash: " << bl.curHash<< "\nPrev Hash:    " << bl.prevHash<< "\nTimestamp:    " << std::to_string(bl.timestamp) << "\nNonce:        " << std::to_string(bl.nonce) << std::endl;
    return os;
}

