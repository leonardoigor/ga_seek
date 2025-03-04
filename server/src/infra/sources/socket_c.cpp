#include "socket_c.hpp"

WebSocketServer::WebSocketServer()
{
    // Set logging settings
    m_server.set_access_channels(websocketpp::log::alevel::all);
    m_server.set_error_channels(websocketpp::log::elevel::all);

    // Initialize ASIO
    m_server.init_asio();

    // Register handlers
    m_server.set_open_handler(std::bind(&WebSocketServer::on_open, this, std::placeholders::_1));
    m_server.set_close_handler(std::bind(&WebSocketServer::on_close, this, std::placeholders::_1));
    m_server.set_message_handler(std::bind(&WebSocketServer::on_message, this,
                                           std::placeholders::_1, std::placeholders::_2));
}

void WebSocketServer::run(uint16_t port)
{
    // Listen on specified port
    m_server.listen(port);

    // Start the server accept loop
    m_server.start_accept();

    // Start the ASIO io_service run loop
    std::cout << "WebSocket server running on port " << port << std::endl;
    m_server.run();
}

void WebSocketServer::on_open(connection_hdl hdl)
{
    std::cout << "New connection opened" << std::endl;
    m_connections.insert(hdl);
}

void WebSocketServer::on_close(connection_hdl hdl)
{
    std::cout << "Connection closed" << std::endl;
    m_connections.erase(hdl);
}

void WebSocketServer::on_message(connection_hdl hdl, server::message_ptr msg)
{
    std::cout << "Message received: " << msg->get_payload() << std::endl;

    // Echo the message back to the client
    m_server.send(hdl, msg->get_payload(), msg->get_opcode());

    // Broadcast message to all connected clients
    broadcast(msg->get_payload(), hdl);
}

void WebSocketServer::broadcast(const std::string &message, connection_hdl hdl)
{
    for (auto conn : m_connections)
    {
        if (!connection_hdl_equals(conn, hdl))
        { // Skip the sender
            m_server.send(conn, message, websocketpp::frame::opcode::text);
        }
    }
}

bool WebSocketServer::connection_hdl_equals(const connection_hdl &a, const connection_hdl &b)
{
    return !a.owner_before(b) && !b.owner_before(a);
}
