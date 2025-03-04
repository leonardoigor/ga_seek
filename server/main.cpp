
#include <iostream>
#include <set>
#include <functional>
#include "socket_c.hpp"
#include "c_game.hpp"

int main()
{
    try
    {
        WebSocketServer server;
        // for (size_t i = 0; i < 5000; i++)
        // {
        //     Game game;
        //     game.update();
        // }

        server.run(9002);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}