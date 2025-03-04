// WebSocketServer.hpp
#ifndef WEBSOCKETSERVER_HPP
#define WEBSOCKETSERVER_HPP

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <set>
#include <iostream>

class WebSocketServer
{
public:
    WebSocketServer();
    void run(uint16_t port);

private:
    using server = websocketpp::server<websocketpp::config::asio>;
    using connection_hdl = websocketpp::connection_hdl;

    void on_open(connection_hdl hdl);
    void on_close(connection_hdl hdl);
    void on_message(connection_hdl hdl, server::message_ptr msg);
    void broadcast(const std::string &message, connection_hdl hdl);
    bool connection_hdl_equals(const connection_hdl &a, const connection_hdl &b);

    server m_server;
    std::set<connection_hdl, std::owner_less<connection_hdl>> m_connections;
};

#endif // WEBSOCKETSERVER_HPP
