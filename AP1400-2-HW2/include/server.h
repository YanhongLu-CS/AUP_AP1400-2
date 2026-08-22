#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <map>
#include <memory>
#include <cstddef>
#include <vector>

extern std::vector<std::string> pending_trxs;

class Client;

class Server
{
public:
	Server();
	std::shared_ptr<Client> add_client(std::string id);
	std::shared_ptr<Client> get_client(std::string id) const;
	double get_wallet(std::string id) const;
	static bool parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value);
	//static means we do not need to create an object, instead, we can use Server directly
	bool add_pending_trx(std::string trx, std::string signature) const;
	size_t mine();
private:
	std::map<std::shared_ptr<Client>, double> clients;
	friend void show_wallets(const Server& server);
};
void show_wallets(const Server& server);
void  show_pending_transactions();
#endif //SERVER_H