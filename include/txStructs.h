#ifndef TX_STRUCTS_H
#define TX_STRUCTS_H

#include "api.h"
#include "BRng.h"
#include <vector>
#include <string>
#include <botan/hash.h>
#include <botan/base58.h>
#include <memory>
#include <json/json.h>

const float CB_AMNT = 250.0;
const int blockDecay = 550000;

namespace transData{

struct TxIn
{
    std::string txOutId,txSig; //The referenced TxOut and the signature signed with he referenced TxOut corresponding private key
    int indexOut;

    const Json::Value toJson()
    {
        Json::Value storedData(Json::objectValue);
        storedData["txOutId"] = txOutId;
        storedData["txSig"] = txSig;
        storedData["indexOut"] = indexOut;
        return storedData;
    }

    void fromJson(Json::Value loadData)
    {
        txOutId = loadData["txOutId"].asString();
        txSig = loadData["txSig"].asString();
        indexOut = loadData["indexOut"].asInt();
    }
};

struct TxOut
{
    std::string addr;
    float amount;

    const Json::Value toJson()
    {
        Json::Value storedData(Json::objectValue);
        storedData["amount"] = amount;
        storedData["address"] = addr;
        return storedData;
    }

    void fromJson(Json::Value loadData)
    {
        addr = loadData["address"].asString();
        amount = loadData["amount"].asFloat();
    }

};

struct unspentTxOut
{
    const float _amount;
    const std::string _address;
    const int _txOutIndex;
    const std::string _txOutId;

    unspentTxOut(std::string CtxOutId, std::string Caddress, int CtxOutIndex, float Camount):_amount(Camount),_address(Caddress),_txOutIndex(CtxOutIndex),_txOutId(CtxOutId)
    {
    }

    unspentTxOut(Json::Value loadData):_amount(loadData["amount"].asFloat()),_address(loadData["pubAddr"].asString()),_txOutIndex(loadData["txOutIndex"].asInt()),_txOutId(loadData["txOutId"].asString())
    {
    }

    const Json::Value toJson()
    {
        Json::Value storedData(Json::objectValue);
        storedData["amount"] = _amount;
        storedData["pubAddr"] = _address;
        storedData["txOutIndex"] = _txOutIndex;
        storedData["txOutId"] = _txOutId;
        return storedData;
    }

};




}

namespace tDat = transData;

#endif
