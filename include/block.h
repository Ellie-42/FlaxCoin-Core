#ifndef BLOCK_H
#define BLOCK_H

#include <json/json.h>

class block
{
    public:

        block(unsigned int index, std::string prevHash, unsigned  int timestamp, std::vector<std::string> data, int diff);
        block(block &clone);

        block(Json::Value jsonData);

        unsigned int getIndex() { return index; }
        /** Access curHash
         * \return The current value of curHash
         */
        std::string getCurHash() { return curHash; }
        /** Set curHash
         * \param val New value to set
         */
        void setCurHash(std::string val) { curHash = val; }
        /** Access prevHash
         * \return The current value of prevHash
         */
        std::string getPrevHash() { return prevHash; }
        /** Access timestamp
         * \return The current value of timestamp
         */
        unsigned int getTimestamp() { return timestamp; }
        /** Access data
         * \return The current value of data
         */
        std::vector<std::string> getData() { return data; }

        void addTxData(std::string);

        std::string combineTxData();

        int nonce;

        int getDiff(){return diff;}

        Json::Value toJson() const;

        std::string digest(int cur_nonce)
        {
            return std::to_string(index) + std::to_string(timestamp) + prevHash + combineTxData() + std::to_string(cur_nonce);
        }

        friend std::ostream& operator<< ( std::ostream& os , const block& bl);

    protected:

    private:
        int diff = 0;
        unsigned int index; //!< Member variable "index"
        std::string curHash; //!< Member variable "curHash"
        std::string prevHash; //!< Member variable "prevHash"
        unsigned  int timestamp; //!< Member variable "timestamp"
        std::vector<std::string> data; //!< Member variable "data", contains stringified JSON data for ease of portability
};

#endif // BLOCK_H
