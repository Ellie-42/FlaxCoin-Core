#include "transactionData.h"

#include <vector>
#include <string>
#include <botan/hash.h>
#include <botan/base58.h>
#include <botan/base64.h>
#include <memory>
#include <json/json.h>
#include "txStructs.h"

using namespace tDat;

std::string transaction::createTxId()
{
    std::string txComp;
    for(TxIn txIn : txInList)
    {
        txComp += txIn.toJson().asString();
    }
    for(TxOut txOut : txOutList)
    {
        txComp += txOut.toJson().asString();
    }
    std::unique_ptr<Botan::HashFunction> hash_01(Botan::HashFunction::create("SHA-256"));
    std::unique_ptr<Botan::HashFunction> hash_02(Botan::HashFunction::create("SKEIN-512(104)"));
    std::string retId = (Botan::base64_encode(Botan::unlock(hash_02->process(hash_01->process(txComp))).data(), 104/8));
    return retId;
}

std::string transaction::signTx(std::shared_ptr<sphincs_plus::key_shake256_256f_simple> key)
{
    if(key->get_private_key() == 0 || id == ""){return "";}

    unsigned char transactionDataBytes[12];
    int datLength = Botan::base64_decode(transactionDataBytes, id);
    std::shared_ptr<b_rng> sigRng(new b_rng());
    std::unique_ptr<unsigned char[]> sigPointer(key->sign(transactionDataBytes, datLength, sigRng));
    std::string sig = Botan::base58_encode(sigPointer.get(),key->len_signature());
    std::cout<<"Sig: "<<sig<<std::endl;
    return sig;
}

Json::Value const transaction::toJson()
{
    Json::Value storedData(Json::objectValue);
    Json::Value txOutData(Json::arrayValue);
    Json::Value txInData(Json::arrayValue);

    txInData[0] = txInList.size();
    for(unsigned int i = 1; i <= this->txInList.size(); i++)
    {
        txInData[i] = txInList.at(i-1).toJson();
    }

    txOutData[0] = txOutList.size();
    for(unsigned int i = 1; i <= this->txOutList.size(); i++)
    {
        txOutData[i] = txOutList.at(i-1).toJson();
    }

    storedData["txInList"] = txInData;
    storedData["txOutList"] = txOutData;
    storedData["txID"] = id;
    storedData["txSignature"] = signature;
    return storedData;
}

void transaction::fromJsonData(Json::Value jsonIn)
{
    loadedJson = true;
    Json::Value txInData = jsonIn["txInList"];
    Json::Value txOutData = jsonIn["txOutList"];
    id = jsonIn["txID"].asString();
    signature = jsonIn["txSignature"].asString();
    for(int i = 1; i <= txInData[0].asInt(); i++)
    {
        TxIn tempTxIn;
        tempTxIn.fromJson(txInData[i]);
        txInList.push_back(tempTxIn);
    }
    for(int i = 1; i <= txOutData[0].asInt(); i++)
    {
        TxOut tempTxOut;
        tempTxOut.fromJson(txOutData[i]);
        txOutList.push_back(tempTxOut);
    }
}

bool transaction::validateTxIn(TxIn txIn, std::vector<unspentTxOut> uTxOut)
{
    bool found = false;
    for(unspentTxOut txOut : uTxOut)
    {
        if(txOut._txOutId == txIn.txOutId && found == false)
        {
            found = true;
        }
        else
        {
            found = false;
        }
    }
    return found;
}

bool transaction::validateTransaction()
{
    /*if(this->getId() != this->createTxId()){return false;}
    for(TxIn txIn : txInList )
    {
        if(this->validateTransaction(txIn,uTxOutList) == false){return false;}
    }
    if(txOutList.size() != txInList.size()){return false;}
    return true;*/
    return false;

}


