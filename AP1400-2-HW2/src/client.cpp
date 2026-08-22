#include "client.h"
#include "crypto.h"
#include "server.h"

#include <random>

Client::Client(std::string id, const Server& server)
    : server(&server), id(id)
    /*
    ali:
    server      -> main_server
    id          = "ali"
    public_key  = ""
    private_key = ""
    then we set the two keys.
    */
{
    crypto::generate_key(public_key, private_key);
}

std::string Client::get_id() const
{
    return id;
}

std::string Client::get_publickey() const
{
    return public_key;
}
double Client::get_wallet() const
{
    return server->get_wallet(id);
    //return (*server).get_wallet(id);
}
std::string Client::sign(std::string txt) const
{
    return crypto::signMessage(private_key, txt);
}
bool Client::transfer_money(std::string receiver, double value) const
{
    std::string trx = this->get_id() + "-" + receiver + "-" + std::to_string(value);
    return this->server->add_pending_trx(trx, sign(trx));
}
size_t Client::generate_nonce() const
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<size_t> dist(0, 1000000000);
    return dist(gen);
}
