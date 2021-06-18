#ifndef TRANSACTIONDATA_H
#define TRANSACTIONDATA_H

#include <vector>
#include <string>
#include <botan/hash.h>
#include <botan/base58.h>
#include <memory>
#include <json/json.h>
#include "txStructs.h"
#include "api.h"

namespace transData{

class transaction
{
    private:
        bool loadedJson = false;
        std::string id;
        std::vector<TxIn> txInList;
        std::vector<TxOut> txOutList;
        std::string signature;

    public:
        transaction(){isEmpty = false;}

        transaction(std::string jsonString)
        {
            Json::Value val(jsonString);
            fromJsonData(val);
        }

        transaction(Json::Value val)
        {
            fromJsonData(val);
        }

        bool isEmpty = false;

        std::string createTxId();

        void setTxId(std::string tId){id = tId;}

        std::string getId(){return id;}
        std::string getSig() {return signature;}

        std::string signTx(std::shared_ptr<sphincs_plus::key_shake256_256f_simple> key);

        bool setSig(std::string sig){if(sig!=""){signature = sig; return true;}return false;}

        const Json::Value toJson();

        void fromJsonData(Json::Value jsonIn);

        bool scanPubAddr(std::string addr);

        bool validateTxIn(TxIn txIn, std::vector<unspentTxOut> uTxOut);

        bool validateTransaction();
};

}
#endif // TRANSACTIONDATA_H
