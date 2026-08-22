#include "server.h"
#include "crypto.h"
#include "client.h"

#include <iostream>
#include <random>
#include <stdexcept>

std::vector<std::string> pending_trxs;

Server::Server()
{

}

std::shared_ptr<Client> Server::get_client(std::string id) const
{
    for (const auto& client : clients) {
        if (client.first->get_id() == id) {
            return client.first;
        }
    }
    return nullptr;
}

std::shared_ptr<Client> Server::add_client(std::string id)
{
    std::string original_id = id;
    if (get_client(id) != nullptr) {
        std::random_device rd;
        //rd is a seed.
        std::mt19937 gen(rd());
        //gen is a generating machine.
        std::uniform_int_distribution<int> dist(1000, 9999);
        while (true) {
            int appended_number = dist(gen);
            std::string appended_string = std::to_string(appended_number);
            id = original_id + appended_string;
            if (get_client(id) == nullptr) {
                break;
            }
        }

    }
    auto new_client = std::make_shared<Client>(id, *this);
    clients[new_client] = 5.0;
    return new_client;
}

double Server::get_wallet(std::string id) const
{
    for (const auto& client : clients) {
        if (client.first->get_id() == id) {
            return client.second;
        }
    }
    return 0.0;
}

void show_wallets(const Server& server)
{
    std::cout << std::string(20, '*') << std::endl;

    for (const auto& client : server.clients) {
        std::cout << client.first->get_id() << " : " << client.second << std::endl;
    }

    std::cout << std::string(20, '*') << std::endl;
}

bool Server::parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value)
{
    size_t first_dash = trx.find('-');
    size_t second_dash = trx.find('-', first_dash + 1);
    if (first_dash == std::string::npos || second_dash == std::string::npos) {
        throw std::runtime_error("wrong trx format");
    }
    sender = trx.substr(0, first_dash);
    receiver = trx.substr(first_dash + 1, second_dash - first_dash - 1);
    value = std::stod(trx.substr(second_dash + 1));
    return true;
}

bool Server::add_pending_trx(std::string trx, std::string signature) const
{
    std::string sender, receiver;
    double value;
    try {
        parse_trx(trx, sender, receiver, value);
    } catch (const std::runtime_error&){
        return false;
    }
    auto sender_client = get_client(sender);
    auto receiver_client = get_client(receiver);
    if (sender_client == nullptr || receiver_client == nullptr) {
        return false;
    }
    if (sender_client->get_wallet() < value || value <= 0) {
        return false;
    }
    if (! crypto::verifySignature(sender_client->get_publickey(), trx, signature)) {
        return false;
    }
    pending_trxs.push_back(trx);
    return true;
}

void  show_pending_transactions()
{
    std::cout  <<  std::string(20, '*') <<  std::endl;
 	for(const  auto& trx : pending_trxs)
 		std::cout << trx <<  std::endl;
 	std::cout  <<  std::string(20, '*') <<  std::endl;
}

size_t Server::mine()
{
    std::string mempool;
    for (const auto& trx : pending_trxs) {
        mempool += trx;
    }
    while (true) {
        for (auto& client : clients) {
            size_t nonce = client.first->generate_nonce();
            std::string hash = crypto::sha256(mempool + std::to_string(nonce));
            if (hash.substr(0,10).find("000") != std::string::npos) {
                std::cout << client.first->get_id() << std::endl;
                client.second +=6.25;
                for(const auto& trx : pending_trxs) {
                    std::string sender, receiver;
                    double value;
                    parse_trx(trx, sender, receiver, value);
                    auto sender_client = get_client(sender);
                    auto receiver_client = get_client(receiver);
                    clients[sender_client] -= value;
                    clients[receiver_client] += value;
                }
                pending_trxs.clear();
                return nonce;
            }
        }
        
    }
}