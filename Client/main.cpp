#include "Screens.h"

bool Init()
{
    // Initialize SDL along with initialize audio
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        BOOST_LOG_TRIVIAL(error) << "SDL could not be initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create Window
    gWindow = SDL_CreateWindow("Homework Submitter...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Window could not be Created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create Window Renderer
    gWindowRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gWindowRenderer == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize renderer color
    SDL_SetRenderDrawColor(gWindowRenderer, 255, 255, 255, 255); // White

    // INitialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        BOOST_LOG_TRIVIAL(error) << "SDL_image could not initialize! SDL Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Initialize TTF
    if (TTF_Init() == -1)
    {
        BOOST_LOG_TRIVIAL(error) << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}

bool Close()
{
    // Free the Renderer
    SDL_DestroyRenderer(gWindowRenderer);
    gWindowRenderer = nullptr;

    // Free the Window
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    // QUIT SDL SUBSYSTEM
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
    return true;
}

int main(int argc, char *argv[])
{
    if (!Init())
    {
        BOOST_LOG_TRIVIAL(error) << "Initialize Fail!" << std::endl;
        return 1;
    }

    // Connection To Server
    boost::asio::io_context io_context;
    std::shared_ptr<boost::asio::ip::tcp::socket> client_socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context);

    std::string serverIPv4AddressStr = "127.0.0.1";
    unsigned short port = 6969;

    try
    {
        boost::asio::ip::tcp::endpoint serverEndpoint(boost::asio::ip::address::from_string(serverIPv4AddressStr), port);
        client_socket->connect(serverEndpoint);

        // Read the initial message from the server
        std::string initialMessage;
        getData(client_socket, initialMessage);
        std::cout << "Received: " << initialMessage << std::endl;
    }
    catch (const std::exception &e)
    {
        BOOST_LOG_TRIVIAL(error) << e.what() << std::endl;
        client_socket->close();
        return 1;
    }
    

    // While the application still running
    PageStatus pageStatus = RenderPageWelcome(); // Initial Page
    bool isRunning = true;
    while (isRunning)
    {
        // Production
        //-------------------------------------------------------------------
        switch (pageStatus)
        {
        case PAGE_SYSTEM_QUIT:
            isRunning = false;

            break;
        case TO_LOGIN_PAGE:
            pageStatus = RenderLoginPage(client_socket);
            break;
        case TO_MENU_PAGE:
            pageStatus = RenderPageMenu();
            break;
        case TO_INPUT_PAGE:
            pageStatus = RenderPageInput();
            break;
        case TO_INPUT_TERMINAL_PAGE:
            pageStatus = RenderPageInputFromTerminal();
            break;
        case TO_INPUT_FILE_PATH:
            //! Temporary
            pageStatus = RenderPageMenu();
            break;
        case TO_DISPLAY_GROUP_PAGE:
            pageStatus = RenderGroupInfoPage();
            break;
        case TO_DISPLAY_PROJECT_PAGE:
            pageStatus = RenderProjectInfoPage();
            break;
        default:
            break;
        }
    }

    // Free The Font
    TTF_CloseFont(textFont);
    textFont = nullptr;

    if (Close())
    {
        BOOST_LOG_TRIVIAL(info) << "Shutdown the Program!" << std::endl;
    }

    for (size_t index = 0; index < projects.size(); index++)
    {
        delete projects[index];
    }
    for (size_t index = 0; index < groups.size(); index++)
    {
        delete groups[index];
    }

    return 0;
}

// int main(int argc, char *argv[])
// {
//     if (!Init())
//     {
//         BOOST_LOG_TRIVIAL(error) << "Initialize Fail!" << std::endl;
//         return 1;
//     }

//     // Dot Character
//     Dot dot;
//     dot.AddTexture("src/Learning/dot.bmp");

//     // Set the Wall
//     SDL_Rect wall = {
//         300,
//         40,
//         40,
//         400,
//     };

//     // Event Handler
//     SDL_Event event;
//     bool isRunning = true;

//     // While application is running
//     while (isRunning)
//     {
//         // Move the Dot
//         dot.Move(wall);

//         // Clear screen
//         SDL_SetRenderDrawColor(gWindowRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
//         SDL_RenderClear(gWindowRenderer);

//         // Render The Dot
//         dot.Render();

//         // Render the Wall
//         SDL_SetRenderDrawColor(gWindowRenderer, 0, 0,0, 255);
//         SDL_RenderFillRect(gWindowRenderer, &wall);

//         // Update The Screen
//         SDL_RenderPresent(gWindowRenderer);

//         SDL_Delay(30 / 1000.f);

//         // Handle events on queue
//         while (SDL_PollEvent(&event) != 0)
//         {
//             // User requests quit
//             if (event.type == SDL_QUIT)
//             {
//                 isRunning = false;
//             }

//             // Handle Input for the Dot
//             dot.HandleEvent(&event);
//         }
//     }

//     if (Close())
//     {
//         BOOST_LOG_TRIVIAL(info) << "Shutdown the Program!" << std::endl;
//     }

//     return 0;
// }

// int main(int argc, char *argv[])
// {
//     boost::asio::io_context io_context;
//     std::shared_ptr<boost::asio::ip::tcp::socket> client_socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context);

//     std::string serverIPv4AddressStr = "127.0.0.1";
//     unsigned short port = 6969;

//     try
//     {
//         boost::asio::ip::tcp::endpoint serverEndpoint(boost::asio::ip::address::from_string(serverIPv4AddressStr), port);
//         client_socket->connect(serverEndpoint);

//         // Read the initial message from the server
//         std::string initialMessage;
//         getData(client_socket, initialMessage);
//         std::cout << "Received: " << initialMessage << std::endl;

//         // Get user_email
//         std::string user_email;
//         std::cout << "Enter Email: ";
//         std::getline(std::cin, user_email);
//         sendData(client_socket, user_email);

//         // Generate RSA public key and RSA private key
//         RSA *publicKey, *privateKey;
//         privateKey = generateRSAKeyPair();
//         publicKey = RSAPrivateKey_dup(privateKey);

//         // Store Private Key into rsa_private_key.pem file
//         writeRSAPrivateKeyToPEMFile(publicKey, "keys/rsa_private_key.pem");

//         // Send the public Key to the Server
//         json public_key_json_object;

//         public_key_json_object.emplace("key", rsaPublicKeyToString(publicKey));
//         sendData(client_socket, &public_key_json_object);

//         // WAIT TILL THE SERVER GRANT PERMISSION TO CONNECT
//         std::cout << "WAIT FOR THE SERVER GRANT PERMISSION TO CONNECT..." << std::endl;
//         std::string status;
//         getData(client_socket, status);

//         if (status == "exit")
//         {
//             isRunning = false;
//         }

//         // Start a thread to receive data from the server
//         std::thread receive_thread([&client_socket]()
//                                    {
//             std::string response;
//             while (isRunning)
//             {
//                 int status = getData(client_socket, response);
//                 if (status == FAIL)
//                 {
//                     break;
//                 }

//                 std::cout << std::endl 
//                         << "From Server: " << response 
//                         << std::endl;

//                 if (response == "CLOSE BY SERVER")
//                 {
//                     isRunning = false;
//                     sendData(client_socket, "CLOSE BY SERVER");
//                     return;
//                 }
//                 else if (response == "exit")
//                 {
//                     isRunning = false;
//                     return;
//                 }
                
//             } });

//         while (isRunning)
//         {
//             std::string message;
//             std::cout << "Enter a message to send (or 'exit' to quit): ";
//             std::getline(std::cin, message);

//             if (message == "exit")
//             {
//                 sendData(client_socket, "exit");
//                 // Close the client_socket when done
//                 receive_thread.join();

//                 continue;
//             }

//             // Try to Encrypt with AES-128
//             // In Production use RSA Algo - 2048 bit
//             std::string encrypted_message;
//             encryptRSA(reinterpret_cast<const unsigned char *>(message.c_str()), publicKey, encrypted_message);

//             sendData(client_socket, encrypted_message);
//         }

//         // Free memory of the public and Private Key
//         RSA_free(publicKey);
//         RSA_free(privateKey);
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Exception: " << e.what() << std::endl;
//     }

//     return 0;
// }
