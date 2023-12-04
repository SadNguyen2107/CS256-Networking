#ifndef SCREENS_H
#define SCREENS_H

//-------------------------------------------------------
// GUI Header File
#include "src/include/SharedResource.h"
#include "src/include/Texture.h"
#include "src/include/Switch.h"
#include "src/include/Button.h"
#include "src/include/TextInputBox.h"
#include "src/Windows/GetFileWindow.h"
#include "src/Base/Group.h"
#include "src/Base/Project.h"
#include "src/Validate/ValidateRegex.h"
#include "src/Validate/ValidateFile.h"
#include "Client.h" // Networking Header Files

// Globally Used Groups
std::vector<Group *> groups = std::vector<Group *>();

// Globally Used Projects
std::vector<Project *> projects = std::vector<Project *>();

// Shared Resources Every Page
// The Milisecond the Screen will be delay
const Uint32 frame_rate = 30;
const Uint32 delay_ms = 1000 / frame_rate;

// Backgroung Setting
SDL_Rect backGroundQuad = {
    0,
    0,
    SCREEN_WIDTH,
    SCREEN_WIDTH};

SDL_Color whiteColor = {240, 242, 245, 255};
SDL_Color blackColor = {42, 48, 60, 255};
SDL_Color *defaultColor;

// Display Text At the Middle
TTF_Font *textFont;

// Switch Dark Mode and Light Mode
Switch switchObj;
//--------------------------------------------------------------

enum Page
{
    PAGE_WELCOME = 0,
    PAGE_MENU = 1,
    PAGE_INPUT = 2,
    TOTAL_PAGES = 3
};

enum PageStatus
{
    PAGE_UNFINISHED = 0,
    PAGE_SYSTEM_QUIT = 1,

    TO_WELCOME_PAGE = 2,
    TO_LOGIN_PAGE = 3, // Login Phase
    TO_MENU_PAGE = 4,

    TO_INPUT_PAGE = 5,
    TO_INPUT_TERMINAL_PAGE = 6,
    TO_INPUT_FILE_PATH = 7,

    TO_DISPLAY_GROUP_PAGE = 8,
    TO_DISPLAY_PROJECT_PAGE = 9,

    PAGE_FINISHED = 10,
    TOTAL_PAGE = 11
};

void SetUp()
{
    defaultColor = &whiteColor;
    switchObj.AddBothSwitch("resources/light_mode.png", "resources/dark_mode.png");
}

PageStatus RenderPageWelcome()
{
    SetUp();
    // Ready For the Resource
    PageStatus pageStatus = PAGE_UNFINISHED;

    // Font of this Page
    if (textFont != nullptr)
    {
        TTF_CloseFont(textFont); // Release old Font
    }
    textFont = TTF_OpenFont("resources/JetBrainsMono-VariableFont_wght.ttf", 50);

    // Switch Object Position
    switchObj.SetPosition(0, 0);

    // Start Button Functionality
    Button startButtonObj;
    startButtonObj.AddButtonAnimations("resources/startbutton_1.png", "resources/startbutton_2.png", "resources/startbutton_3.png");
    startButtonObj.SetPosition(572, 361);

    // Welcome Text
    Texture welcomeTextTexture;
    std::string welcomeText = "Welcome to CS256 Project";
    welcomeTextTexture.LoadFromRenderedText(welcomeText, textFont, {255, 0, 255, 255});
    welcomeTextTexture.SetPosition(250, 265);

    // Render to the Screen
    // While Screen1 is still Running
    bool isRunning = true;
    SDL_Event event;
    while (isRunning)
    {
        // Render the Current Screen
        SDL_SetRenderDrawColor(gWindowRenderer, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a); // Merely Black
        SDL_RenderClear(gWindowRenderer);

        // Render Select BackGround
        SDL_SetRenderDrawColor(gWindowRenderer, defaultColor->r, defaultColor->g, defaultColor->b, defaultColor->a);
        SDL_RenderFillRect(gWindowRenderer, &backGroundQuad);

        // Render the Switch
        switchObj.Render();

        // Render the Text
        welcomeTextTexture.Render();

        // Render the Start Button
        startButtonObj.Render();

        // Update the Screen
        SDL_RenderPresent(gWindowRenderer);

        // Handle Events on queue
        while (SDL_PollEvent(&event) != 0)
        {
            // User Requests quit
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
                pageStatus = PAGE_SYSTEM_QUIT;
                break;
            }

            int switchStatus = switchObj.HandleEvent(&event);
            if (switchStatus == SWITCH_SPRITE_OFF)
            {
                defaultColor = &blackColor;
            }
            else if (switchStatus == SWITCH_SPRITE_ON)
            {
                defaultColor = &whiteColor;
            }

            ButtonSpriteState startbuttonState = startButtonObj.HandleEvent(&event);
            if (startbuttonState == BUTTON_SPRITE_MOUSE_DOWN)
            {
                isRunning = false;
                pageStatus = TO_LOGIN_PAGE;
            }
        }

        // Delay Some Microsecond
        SDL_Delay(delay_ms);
    }

    // Free The Resource
    welcomeTextTexture.FreeTexture();

    return pageStatus;
}

PageStatus RenderLoginPage(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket)
{
    // Ready for the Resouce
    PageStatus pageStatus = PAGE_UNFINISHED;

    // Font of this Page
    if (textFont != nullptr)
    {
        TTF_CloseFont(textFont); // Release old Font
    }
    textFont = TTF_OpenFont("resources/JetBrainsMono-VariableFont_wght.ttf", 50);

    // Switch Object Position
    switchObj.SetPosition(0, 0);

    // Login to your account Text
    Texture loginTextTexture;
    std::string loginText = "Login to your account";
    loginTextTexture.LoadFromRenderedText(loginText, textFont, {255, 0, 255, 255});
    loginTextTexture.SetPosition(409, 131);

    // ID Text
    Texture idTextTexture;
    std::string idText = "ID:";
    idTextTexture.LoadFromRenderedText(idText, textFont, {255, 0, 255, 255});
    idTextTexture.SetPosition(414, 309);

    // TextBox to input ID
    TextInputBox idTextBox;
    idTextBox.AddFont(textFont);
    idTextBox.AddTextColor({255, 0, 255, 255});
    idTextBox.AddTexture("Input Your HUST-ID...");
    idTextBox.SetPosition(553, 309);

    // Render to the Screen
    // While Screen1 is still Running
    bool isRunning = true;
    SDL_Event event;
    while (isRunning)
    {
        // Render the Current Screen
        SDL_SetRenderDrawColor(gWindowRenderer, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a); // Merely Black
        SDL_RenderClear(gWindowRenderer);

        // Render Select BackGround
        SDL_SetRenderDrawColor(gWindowRenderer, defaultColor->r, defaultColor->g, defaultColor->b, defaultColor->a);
        SDL_RenderFillRect(gWindowRenderer, &backGroundQuad);

        // Render the Switch
        switchObj.Render();

        // Render Login Text
        loginTextTexture.Render();

        // Render ID text and ID TextBox
        idTextTexture.Render(); // Render ID Text
        idTextBox.Render();     // Render ID TextBox

        // Update the Screen
        SDL_RenderPresent(gWindowRenderer);

        // Handle Events on queue
        while (SDL_PollEvent(&event) != 0)
        {
            // User Requests quit
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
                pageStatus = PAGE_SYSTEM_QUIT;
                break;
            }

            int switchStatus = switchObj.HandleEvent(&event);
            if (switchStatus == SWITCH_SPRITE_OFF)
            {
                defaultColor = &blackColor;
            }
            else if (switchStatus == SWITCH_SPRITE_ON)
            {
                defaultColor = &whiteColor;
            }

            // Handle Event for ID TextBox
            TextInputBoxState idTextBoxState = idTextBox.HandleEvent(&event);
            if (idTextBoxState == TEXT_INPUT_BOX_SPRITE_ENTER)
            {
                std::string hustID_String = idTextBox.GetResult();
                if (validateHustID(hustID_String))
                {
                    // Send Hust ID
                    sendData(client_socket, hustID_String);

                    // WAIT TILL THE SERVER GRANT PERMISSION TO CONNECT
                    std::cout << "WAIT FOR THE SERVER GRANT PERMISSION TO CONNECT..." << std::endl;
                    std::string status;
                    getData(client_socket, status);

                    if (status != "exit")
                    {
                        isRunning = false;
                        pageStatus = TO_MENU_PAGE;

                        // Send RSA Keys To The Server And Save the Key To the client
                        // To Retrieve After Ward
                        GenerateRSAkeys_and_Send(client_socket);
                        break;
                    }
                    else
                    {
                        isRunning = false;
                        pageStatus = TO_LOGIN_PAGE;
                        break;
                    }
                }
            }
        }
        // Delay Some Microsecond
        SDL_Delay(delay_ms);
    }

    return pageStatus;
}

PageStatus RenderPageMenu()
{
    // Ready for the Resouce
    PageStatus pageStatus = PAGE_UNFINISHED;

    // Font of this Page
    if (textFont != nullptr)
    {
        TTF_CloseFont(textFont); // Release old Font
    }
    textFont = TTF_OpenFont("resources/JetBrainsMono-VariableFont_wght.ttf", 50);

    // Switch Object Position
    switchObj.SetPosition(0, 0);

    // Menu Text
    Texture menuTextTexture;
    std::string menuText = "MENU";
    menuTextTexture.LoadFromRenderedText(menuText, textFont, {255, 0, 255, 255});
    menuTextTexture.SetPosition(638, 93);

    // Group Info Text
    Texture groupInputTextTexture;
    std::string groupInputText = "Group Input";
    groupInputTextTexture.LoadFromRenderedText(groupInputText, textFont, {255, 0, 255, 255});
    groupInputTextTexture.SetPosition(220, 203);

    // Display Text
    Texture displayTextTexture;
    std::string displayText = "Display";
    displayTextTexture.LoadFromRenderedText(displayText, textFont, {255, 0, 255, 255});
    displayTextTexture.SetPosition(926, 203);

    // Line At The Middle
    SDL_Rect seperate_line = {
        709,
        181,
        2,
        281};

    // Input Group Button Functionality
    Button inputButtonObj;
    inputButtonObj.AddButtonAnimations("resources/inputbutton_1.png", "resources/inputbutton_2.png", "resources/inputbutton_3.png");
    inputButtonObj.SetPosition(222, 321);

    // Display Project Button Functionality
    Button displayProjectButtonObj;
    displayProjectButtonObj.AddButtonAnimations("resources/projectbutton_1.png", "resources/projectbutton_2.png", "resources/projectbutton_3.png");
    displayProjectButtonObj.SetPosition(752, 321);

    // Display Group Button Functionality
    Button displayGroupButtonObj;
    displayGroupButtonObj.AddButtonAnimations("resources/groupbutton_1.png", "resources/groupbutton_2.png", "resources/groupbutton_3.png");
    displayGroupButtonObj.SetPosition(1072, 321);

    // Render to the Screen
    // While Screen1 is still Running
    bool isRunning = true;
    SDL_Event event;
    while (isRunning)
    {
        // Render the Current Screen
        SDL_SetRenderDrawColor(gWindowRenderer, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a);
        SDL_RenderClear(gWindowRenderer);

        // Render Select BackGround
        SDL_SetRenderDrawColor(gWindowRenderer, defaultColor->r, defaultColor->g, defaultColor->b, defaultColor->a);
        SDL_RenderFillRect(gWindowRenderer, &backGroundQuad);

        // Render the Switch
        switchObj.Render();

        // Render the Text
        menuTextTexture.Render();

        // Render the Group Info Text
        groupInputTextTexture.Render();

        // Render the Input Button
        inputButtonObj.Render();

        // Render The Line At The Between
        SDL_SetRenderDrawColor(gWindowRenderer, 255, 0, 255, 255);
        SDL_RenderFillRect(gWindowRenderer, &seperate_line);

        // Render The Display Text
        displayTextTexture.Render();

        // IF the user has Group?
        // if (hasGroup)
        {
            // Render the Project Button
            displayProjectButtonObj.Render();

            // Render The Display Group Button
            displayGroupButtonObj.Render();
        }

        // Update the Screen
        SDL_RenderPresent(gWindowRenderer);

        // Handle Events on queue
        while (SDL_PollEvent(&event) != 0)
        {
            // User Requests quit
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
                pageStatus = PAGE_SYSTEM_QUIT;
                break;
            }

            int switchStatus = switchObj.HandleEvent(&event);
            if (switchStatus == SWITCH_SPRITE_OFF)
            {
                defaultColor = &blackColor;
            }
            else if (switchStatus == SWITCH_SPRITE_ON)
            {
                defaultColor = &whiteColor;
            }

            // Handle Event For the Input Button
            ButtonSpriteState inputButtonState = inputButtonObj.HandleEvent(&event);
            if (inputButtonState == BUTTON_SPRITE_MOUSE_DOWN)
            {
                isRunning = false;
                pageStatus = TO_INPUT_PAGE;
                break;
            }

            // Handle Event For the Display Project Button
            ButtonSpriteState displayProjectButtonState = displayProjectButtonObj.HandleEvent(&event);
            if (displayProjectButtonState == BUTTON_SPRITE_MOUSE_DOWN)
            {
                isRunning = false;
                pageStatus = TO_DISPLAY_PROJECT_PAGE;
                break;
            }

            // Handle Event For the Display Group Button
            ButtonSpriteState displayGroupButtonState = displayGroupButtonObj.HandleEvent(&event);
            if (displayGroupButtonState == BUTTON_SPRITE_MOUSE_DOWN)
            {
                isRunning = false;
                pageStatus = TO_DISPLAY_GROUP_PAGE;
                break;
            }
        }

        // Delay Some Microsecond
        SDL_Delay(delay_ms);
    }

    return pageStatus;
}

PageStatus RenderPageInput()
{
    // Ready For the Resource
    PageStatus pageStatus = PAGE_UNFINISHED;

    // Font of this Page
    if (textFont != nullptr)
    {
        TTF_CloseFont(textFont); // Release old Font
    }
    textFont = TTF_OpenFont("resources/JetBrainsMono-Italic-VariableFont_wght.ttf", 50);

    // Switch Object Position
    switchObj.SetPosition(0, 0);

    // Welcome Text
    Texture askUserTextTexture;
    std::string askUserText = "USE DATA FROM:";
    askUserTextTexture.LoadFromRenderedText(askUserText, textFont, {255, 0, 255, 255});
    askUserTextTexture.SetPosition(291, 290);

    // File Button Functionality
    Button fileButtonObj;
    fileButtonObj.AddButtonAnimations("resources/filebutton_1.png", "resources/filebutton_2.png", "resources/filebutton_3.png");
    fileButtonObj.SetPosition(898, 256);

    // Terminal Button Functionality
    Button terminalButtonObj;
    terminalButtonObj.AddButtonAnimations("resources/terminalbutton_1.png", "resources/terminalbutton_2.png", "resources/terminalbutton_3.png");
    terminalButtonObj.SetPosition(898, 419);

    // Render to the Screen
    // While Screen1 is still Running
    bool isRunning = true;
    SDL_Event event;
    while (isRunning)
    {
        // Render the Current Screen
        SDL_SetRenderDrawColor(gWindowRenderer, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a); // Merely Black
        SDL_RenderClear(gWindowRenderer);

        // Render Select BackGround
        SDL_SetRenderDrawColor(gWindowRenderer, defaultColor->r, defaultColor->g, defaultColor->b, defaultColor->a);
        SDL_RenderFillRect(gWindowRenderer, &backGroundQuad);

        // Render the Switch
        switchObj.Render();

        // Render the Text
        askUserTextTexture.Render();

        // Render the File Button
        fileButtonObj.Render();

        // Render the Terminal Button
        terminalButtonObj.Render();

        // Update the Screen
        SDL_RenderPresent(gWindowRenderer);

        // Handle Events on queue
        while (SDL_PollEvent(&event) != 0)
        {
            // User Requests quit
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
                pageStatus = PAGE_SYSTEM_QUIT;
                break;
            }

            int switchStatus = switchObj.HandleEvent(&event);
            if (switchStatus == SWITCH_SPRITE_OFF)
            {
                defaultColor = &blackColor;
            }
            else if (switchStatus == SWITCH_SPRITE_ON)
            {
                defaultColor = &whiteColor;
            }

            ButtonSpriteState fileButtonState = fileButtonObj.HandleEvent(&event);
            if (fileButtonState == BUTTON_SPRITE_MOUSE_DOWN)
            {
                isRunning = false;

#ifdef _WIN32
                // Get The File Path Through Open File Dialog (Windows Only)
                std::string filePath;
                int status = GetFileTxtPathWindow(filePath);

                // IF No File was input -> FAIL
                if (status == FAIL)
                {
                    isRunning = false;
                    pageStatus = TO_INPUT_PAGE;
                    break;
                }

                // IF That File Was Correct
                // Validate That File
                if (isRightFile(filePath, GROUP_INFO_FILE))
                {
                    // Extract That Data From File
                    extractGroupInfoFile(&groups, filePath);
                    isRunning = false;
                    pageStatus = TO_MENU_PAGE;
                    break;
                }
#endif
            }

            ButtonSpriteState terminalButtonState = terminalButtonObj.HandleEvent(&event);
            if (terminalButtonState == BUTTON_SPRITE_MOUSE_DOWN)
            {
                isRunning = false;
                pageStatus = TO_INPUT_TERMINAL_PAGE;
                break;
            }
        }

        // Delay Some Microsecond
        SDL_Delay(delay_ms);
    }

    return pageStatus;
}

PageStatus RenderPageInputFromTerminal()
{
    // Ready for the Resouce
    PageStatus pageStatus = PAGE_UNFINISHED;

    // Font of this Page
    if (textFont != nullptr)
    {
        TTF_CloseFont(textFont); // Release old Font
    }
    textFont = TTF_OpenFont("resources/JetBrainsMono-VariableFont_wght.ttf", 50);

    // Switch Object Position
    switchObj.SetPosition(0, 0);

    // Group Info Text
    // Banner
    Texture groupInfoTextTexture;
    std::stringstream groupInfoText;
    int groupIndex = groups.size() + 1; // Get number groupIndex
    groupInfoText << "Group " << groupIndex << " Information:";
    groupInfoTextTexture.LoadFromRenderedText(groupInfoText.str(), textFont, {255, 0, 255, 255});
    groupInfoTextTexture.SetPosition(389, 57);

    //! Group Name Relatted
    // Group Name Text
    Texture groupNameTexture;
    std::string groupName = "Name: ";
    groupNameTexture.LoadFromRenderedText(groupName, textFont, {255, 0, 255, 255});
    groupNameTexture.SetPosition(75, 165);

    // TextBox to input name
    TextInputBox groupNameTextBox;
    groupNameTextBox.AddFont(textFont);
    groupNameTextBox.AddTextColor({255, 0, 255, 255});
    groupNameTextBox.AddTexture("Input Name...");
    groupNameTextBox.SetPosition(262, 165);

    //! Group Members Related
    Texture numOfMembersTexture;
    std::string numOfMembers = "Number Members: ";
    numOfMembersTexture.LoadFromRenderedText(numOfMembers, textFont, {255, 0, 255, 255});
    numOfMembersTexture.SetPosition(75, 222);

    // Textbox to Input
    TextInputBox numOfMemberTextBox;
    numOfMemberTextBox.AddFont(textFont);
    numOfMemberTextBox.AddTextColor({255, 0, 255, 255});
    numOfMemberTextBox.AddTexture("Input Number...");
    numOfMemberTextBox.SetPosition(540, 222);

    // List of Students ID (Display)
    std::vector<std::shared_ptr<Texture>> list_of_students_ID;
    std::vector<std::shared_ptr<TextInputBox>> list_of_student_name;

    // Save Button Functionality
    Button saveButtonObj;
    saveButtonObj.AddButtonAnimations("resources/savebutton_1.png", "resources/savebutton_2.png", "resources/savebutton_3.png");
    saveButtonObj.SetPosition(SCREEN_WIDTH - saveButtonObj.GetWidth(), SCREEN_HEIGHT - saveButtonObj.GetHeight());
    bool isButtonReadyToShow = false;

    // Render to the Screen
    // While Screen1 is still Running
    bool isRunning = true;
    SDL_Event event;
    while (isRunning)
    {
        // Render the Current Screen
        SDL_SetRenderDrawColor(gWindowRenderer, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a); // Merely Black
        SDL_RenderClear(gWindowRenderer);

        // Render Select BackGround
        SDL_SetRenderDrawColor(gWindowRenderer, defaultColor->r, defaultColor->g, defaultColor->b, defaultColor->a);
        SDL_RenderFillRect(gWindowRenderer, &backGroundQuad);

        // Render the Switch
        switchObj.Render();

        // Render Group Info Text
        // Banner
        groupInfoTextTexture.Render();

        // Group Name
        groupNameTexture.Render(); // Render Group Name Text
        groupNameTextBox.Render(); // Render Group Name TextBox

        // Group Members
        numOfMembersTexture.Render();
        numOfMemberTextBox.Render();

        // Render The List For the User to Input the Student Name - ID
        for (size_t index = 0; index < list_of_students_ID.size(); index++)
        {
            list_of_students_ID[index]->Render();
            list_of_student_name[index]->Render();
        }

        // Render The Save Button
        // check if Ready to Render?
        std::string groupName = groupNameTextBox.GetResult();
        for (size_t index = 0; index < list_of_student_name.size(); index++)
        {
            std::string text = list_of_student_name[index]->GetResult();
            if (!validateStudentName_ID(text) || text == "Student_Name - ID" || groupName == "Input Name...")
            {
                isButtonReadyToShow = false;
                break;
            }
            isButtonReadyToShow = true;
        }

        if (isButtonReadyToShow)
        {
            saveButtonObj.Render();
        }

        // Update the Screen
        SDL_RenderPresent(gWindowRenderer);

        // Handle Events on queue
        while (SDL_PollEvent(&event) != 0)
        {
            // User Requests quit
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
                pageStatus = PAGE_SYSTEM_QUIT;
                break;
            }

            int switchStatus = switchObj.HandleEvent(&event);
            if (switchStatus == SWITCH_SPRITE_OFF)
            {
                defaultColor = &blackColor;
            }
            else if (switchStatus == SWITCH_SPRITE_ON)
            {
                defaultColor = &whiteColor;
            }

            // Handle Event for Group Name
            groupNameTextBox.HandleEvent(&event);

            TextInputBoxState numOfMembersTextBoxState = numOfMemberTextBox.HandleEvent(&event);
            if (numOfMembersTextBoxState >= TEXT_INPUT_BOX_SPRITE_INPUT)
            {
                std::string numOfMembers_String = numOfMemberTextBox.GetResult();
                if (validateNum(numOfMembers_String))
                {
                    int numOfMembers_Int = std::atoi(numOfMembers_String.c_str());

                    // Resize the Size respetiveLy to the number of members
                    // To Render the Student ID
                    list_of_students_ID.resize(numOfMembers_Int);
                    int initial_x_ID = 75;
                    int initial_y_ID = 275;
                    for (int index = 0; index < numOfMembers_Int; index++)
                    {
                        list_of_students_ID[index] = std::make_shared<Texture>();
                        std::stringstream student_id;
                        student_id << index + 1;
                        list_of_students_ID[index]->LoadFromRenderedText(student_id.str(), textFont, {255, 0, 255, 255});
                        list_of_students_ID[index]->SetPosition(initial_x_ID, initial_y_ID);
                        initial_y_ID += 70;
                    }

                    // Resize the Size respetiveLy to the number of members
                    // To Render the TextBox for Input Student Name
                    list_of_student_name.resize(numOfMembers_Int);
                    int initial_x = 165;
                    int initial_y = 275;
                    for (int index = 0; index < numOfMembers_Int; index++)
                    {
                        list_of_student_name[index] = std::make_shared<TextInputBox>();
                        list_of_student_name[index]->AddFont(textFont);
                        list_of_student_name[index]->AddTextColor({255, 0, 255, 255});
                        list_of_student_name[index]->AddTexture("Student_Name - ID");
                        list_of_student_name[index]->SetPosition(initial_x, initial_y);
                        initial_y += 70;
                    }
                }
            }

            // Handle Event For The List Of Student Name
            for (size_t index = 0; index < list_of_student_name.size(); index++)
            {
                list_of_student_name[index]->HandleEvent(&event);
            }

            // Handle Event For the Save Button
            ButtonSpriteState saveButtonState = saveButtonObj.HandleEvent(&event);
            if (saveButtonState == BUTTON_SPRITE_MOUSE_DOWN && isButtonReadyToShow)
            {
                isRunning = false;
                pageStatus = TO_MENU_PAGE;

                // Save Data To Vector of Groups
                Group *newGroup = new Group(groupNameTextBox.GetResult());
                for (size_t index = 0; index < list_of_student_name.size(); index++)
                {

                    // Extract That Data
                    std::stringstream text;
                    text.str(list_of_student_name[index]->GetResult());

                    std::string student_name;
                    std::string student_id;
                    char delimiter;

                    // Extract to every Variable
                    text >> student_name >> delimiter >> student_id;

                    // Put Data into Group
                    Student *student = newStudent(student_name, std::stoi(student_id));
                    newGroup->addStudent(student);
                }

                // Add new Group To the groups
                groups.push_back(newGroup);

                break;
            }
        }
        // Delay Some Microsecond
        SDL_Delay(delay_ms);
    }

    return pageStatus;
}

PageStatus RenderGroupInfoPage()
{
    // Ready for the Resouce
    PageStatus pageStatus = PAGE_UNFINISHED;

    // Font of this Page
    if (textFont != nullptr)
    {
        TTF_CloseFont(textFont); // Release old Font
    }
    textFont = TTF_OpenFont("resources/JetBrainsMono-VariableFont_wght.ttf", 50);

    // Switch Object Position
    switchObj.SetPosition(0, 0);

    // Group Info Text
    // Banner
    Texture groupInfoTextTexture;
    std::stringstream groupInfoText;
    int groupIndex = 0; //! Get number groupIndex throuuugh Server
    groupInfoText << "Group " << groupIndex + 1 << " Information:";
    groupInfoTextTexture.LoadFromRenderedText(groupInfoText.str(), textFont, {255, 0, 255, 255});
    groupInfoTextTexture.SetPosition(352, 62);

    //! Group Name Relatted
    // Group Name Text
    Texture groupNameTexture;
    std::stringstream groupNameText;
    groupNameText << "Name: " << groups[groupIndex]->getGroupName();
    groupNameTexture.LoadFromRenderedText(groupNameText.str(), textFont, {255, 0, 255, 255});
    groupNameTexture.SetPosition(355, 111);

    // Text Format Students - IDs
    Texture formatStudents_IDTexture;
    std::string prompt = "Students - IDs";
    formatStudents_IDTexture.LoadFromRenderedText(prompt, textFont, {255, 0, 255, 255});
    formatStudents_IDTexture.SetPosition(85, 212);

    // List of Students Name - ID (Display)
    std::vector<std::shared_ptr<Texture>> list_of_students_name_ID;

    // Get The Information from the Vector
    std::vector<Student> students_list = groups[groupIndex]->getGroupStudentCopy();
    size_t numOfStudents = students_list.size();
    int initialPosX = 85;
    int initialPosY = 258;

    // Resize According to the students_listText
    list_of_students_name_ID.resize(numOfStudents);
    for (size_t index = 0; index < numOfStudents; index++)
    {
        // Create Texture Based On The students_list of Name and ID
        list_of_students_name_ID[index] = std::make_shared<Texture>();
        std::stringstream studentName_Id;
        studentName_Id << index + 1 << ".  " << students_list[index].student_name << " - " << students_list[index].student_id;
        list_of_students_name_ID[index]->LoadFromRenderedText(studentName_Id.str(), textFont, {255, 0, 255, 255});
        list_of_students_name_ID[index]->SetPosition(initialPosX, initialPosY);

        // Increase For the Next Line
        initialPosY += 52;
    }

    // Back Button Functionality
    Button backButtonObj;
    backButtonObj.AddButtonAnimations("resources/backbutton_1.png", "resources/backbutton_2.png", "resources/backbutton_3.png");
    backButtonObj.SetPosition(0, SCREEN_HEIGHT - backButtonObj.GetHeight());

    // Render to the Screen
    // While Screen1 is still Running
    bool isRunning = true;
    SDL_Event event;
    while (isRunning)
    {
        // Render the Current Screen
        SDL_SetRenderDrawColor(gWindowRenderer, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a); // Merely Black
        SDL_RenderClear(gWindowRenderer);

        // Render Select BackGround
        SDL_SetRenderDrawColor(gWindowRenderer, defaultColor->r, defaultColor->g, defaultColor->b, defaultColor->a);
        SDL_RenderFillRect(gWindowRenderer, &backGroundQuad);

        // Render the Switch
        switchObj.Render();

        // Render Group Info Text
        // Banner
        groupInfoTextTexture.Render();

        // Group Name
        groupNameTexture.Render(); // Render Group Name Text

        // Render the Format Student - IDs
        formatStudents_IDTexture.Render();

        // Render The List Of Student Name - ID Retrive From Server
        for (size_t index = 0; index < numOfStudents; index++)
        {
            list_of_students_name_ID[index]->Render();
        }

        // Render The Back Button
        backButtonObj.Render();

        // Update the Screen
        SDL_RenderPresent(gWindowRenderer);

        // Handle Events on queue
        while (SDL_PollEvent(&event) != 0)
        {
            // User Requests quit
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
                pageStatus = PAGE_SYSTEM_QUIT;
                break;
            }

            // Handle Event For the Switch Button
            int switchStatus = switchObj.HandleEvent(&event);
            if (switchStatus == SWITCH_SPRITE_OFF)
            {
                defaultColor = &blackColor;
            }
            else if (switchStatus == SWITCH_SPRITE_ON)
            {
                defaultColor = &whiteColor;
            }

            // Handle Event The Back Button
            ButtonSpriteState backButtonState = backButtonObj.HandleEvent(&event);
            if (backButtonState == BUTTON_SPRITE_MOUSE_DOWN)
            {
                isRunning = false;
                pageStatus = TO_MENU_PAGE;
                break;
            }
        }
        // Delay Some Microsecond
        SDL_Delay(delay_ms);
    }

    return pageStatus;
}

PageStatus RenderProjectInfoPage()
{
    // Ready for the Resouce
    PageStatus pageStatus = PAGE_UNFINISHED;

    // Font of this Page
    if (textFont != nullptr)
    {
        TTF_CloseFont(textFont); // Release old Font
    }
    textFont = TTF_OpenFont("resources/JetBrainsMono-VariableFont_wght.ttf", 50);

    // Switch Object Position
    switchObj.SetPosition(0, 0);

    // Project Info Text
    // Banner
    Texture projectInfoTextTexture;
    std::string getProjectIDText = "Enter project ID: ";
    projectInfoTextTexture.LoadFromRenderedText(getProjectIDText, textFont, {255, 0, 255, 255});
    projectInfoTextTexture.SetPosition(363, 66);

    // TextBox To Input Project ID
    TextInputBox projectIDTextBox;
    projectIDTextBox.AddFont(textFont);
    projectIDTextBox.AddTextColor({255, 0, 255, 255});
    projectIDTextBox.AddTexture("Project ID here...");
    projectIDTextBox.SetPosition(864, 66);

    //! Project Relatted
    // Project Description Text
    Texture projectDescriptionTexture;
    std::string projectDescription = "Description: ";
    projectDescriptionTexture.LoadFromRenderedText(projectDescription, textFont, {255, 0, 255, 255});
    projectDescriptionTexture.SetPosition(64, 232);

    // Project Due Date Text
    Texture projectDueDateTexture;
    std::string projectDueDate = "Due Date: ";
    projectDueDateTexture.LoadFromRenderedText(projectDueDate, textFont, {255, 0, 255, 255});
    projectDueDateTexture.SetPosition(64, 327);

    // Project Submission Status Text
    Texture submissionStatusTexture;
    std::string projectSubmissionStatus = "Submission Status: ";
    submissionStatusTexture.LoadFromRenderedText(projectSubmissionStatus, textFont, {255, 0, 255, 255});
    submissionStatusTexture.SetPosition(64, 422);

    // Back Button Functionality
    Button backButtonObj;
    backButtonObj.AddButtonAnimations("resources/backbutton_1.png", "resources/backbutton_2.png", "resources/backbutton_3.png");
    backButtonObj.SetPosition(0, SCREEN_HEIGHT - backButtonObj.GetHeight());

    // Submit Button Functionality
    bool isSubmit = true; // To Check Whether Is Submit Or Not To Render Submit Button
    Button submitButtonObj;
    submitButtonObj.AddButtonAnimations("resources/submitbutton_1.png", "resources/submitbutton_2.png", "resources/submitbutton_3.png");
    submitButtonObj.SetPosition(SCREEN_WIDTH - submitButtonObj.GetWidth(), SCREEN_HEIGHT - submitButtonObj.GetHeight());

    // Create Assumption Project
    Project *project1 = new Project("Hello World", new Date(21, 10, 2025));
    Project *project2 = new Project("Hello World 2", new Date(29, 10, 2027));
    projects.push_back(project1);
    projects.push_back(project2);

    // Render to the Screen
    // While Screen1 is still Running
    bool isRunning = true;
    SDL_Event event;
    while (isRunning)
    {
        // Render the Current Screen
        SDL_SetRenderDrawColor(gWindowRenderer, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a); // Merely Black
        SDL_RenderClear(gWindowRenderer);

        // Render Select BackGround
        SDL_SetRenderDrawColor(gWindowRenderer, defaultColor->r, defaultColor->g, defaultColor->b, defaultColor->a);
        SDL_RenderFillRect(gWindowRenderer, &backGroundQuad);

        // Render the Switch
        switchObj.Render();

        // Render Group Info Text
        // Banner
        projectInfoTextTexture.Render();

        // Render ProjectID Textbox Name
        projectIDTextBox.Render();

        // Render the Project Description
        projectDescriptionTexture.Render();

        // Render The Project Due Date
        projectDueDateTexture.Render();

        // Render The Submission Status
        submissionStatusTexture.Render();

        // Render The Back Button
        backButtonObj.Render();

        // Render The Submit Button
        if (!isSubmit)
        {
            submitButtonObj.Render();
        }

        // Update the Screen
        SDL_RenderPresent(gWindowRenderer);

        // Handle Events on queue
        while (SDL_PollEvent(&event) != 0)
        {
            // User Requests quit
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
                pageStatus = PAGE_SYSTEM_QUIT;
                break;
            }

            // Handle Event For the Switch Button
            int switchStatus = switchObj.HandleEvent(&event);
            if (switchStatus == SWITCH_SPRITE_OFF)
            {
                defaultColor = &blackColor;
            }
            else if (switchStatus == SWITCH_SPRITE_ON)
            {
                defaultColor = &whiteColor;
            }

            // Handle Event For The Project ID TextBox
            TextInputBoxState projectIDTextBoxState = projectIDTextBox.HandleEvent(&event);
            if (projectIDTextBoxState >= TEXT_INPUT_BOX_SPRITE_INPUT)
            {
                std::string projectID_String = projectIDTextBox.GetResult();
                if (validateNum(projectID_String))
                {
                    // Transform it into numeric
                    int project_index = std::stoi(projectID_String) - 1;
                    if (project_index >= 0 && project_index < projects.size())
                    {
                        projectDescription = projects[project_index]->getDescription();

                        Date *dueDate = projects[project_index]->getDueDate();
                        std::stringstream dueDateText;
                        dueDateText << dueDate->getDay() << "/" << dueDate->getMonth() << "/" << dueDate->getYear();

                        std::vector<Date *> projectSubmitDates = projects[project_index]->getSubmissionDateCopy();
                        //! FIX
                        Date *group1SubmitDate = nullptr;
                        if (1 < projectSubmitDates.size())
                        {
                            group1SubmitDate = projectSubmitDates[0];
                        }

                        std::stringstream group1SubmitDateText;
                        if (group1SubmitDate != nullptr)
                        {
                            group1SubmitDateText << group1SubmitDate->getDay() << "/" << group1SubmitDate->getMonth() << "/" << group1SubmitDate->getYear();
                        }
                        else
                        {
                            group1SubmitDateText << "NULL";
                        }

                        // Change The Content Of the Description
                        projectDescriptionTexture.LoadFromRenderedText("Description: " + projectDescription, textFont, {255, 0, 255, 255});
                        projectDescriptionTexture.SetPosition(64, 232);
                        // Change The Content Of the Due Date
                        projectDueDateTexture.LoadFromRenderedText("Due Date: " + dueDateText.str(), textFont, {255, 0, 255, 255});
                        projectDueDateTexture.SetPosition(64, 327);
                        // Change The Content Of the Submission Status
                        submissionStatusTexture.LoadFromRenderedText("Submission Status: " + group1SubmitDateText.str(), textFont, {255, 0, 255, 255});
                        submissionStatusTexture.SetPosition(64, 422);

                        // Check If We Have To Render Submit button
                        if (checkState(dueDate, group1SubmitDate) == "NULL")
                        {
                            isSubmit = false;
                        }
                        else
                        {
                            isSubmit = true;
                        }

                        // Free The Memory
                        for (size_t index = 0; index < projectSubmitDates.size(); index++)
                        {
                            delete projectSubmitDates[index];
                        }
                    }
                }
            }

            // Handle Event The Back Button
            ButtonSpriteState backButtonState = backButtonObj.HandleEvent(&event);
            if (backButtonState == BUTTON_SPRITE_MOUSE_DOWN)
            {
                isRunning = false;
                pageStatus = TO_MENU_PAGE;
                break;
            }

            // Handle Event For the Submit Button
            if (!isSubmit)
            {
                ButtonSpriteState submitButtonState = submitButtonObj.HandleEvent(&event);
                if (submitButtonState == BUTTON_SPRITE_MOUSE_DOWN)
                {
#ifdef _WIN32
                    // Get The File Path Through Open File Dialog (Windows Only)
                    std::string filePath;
                    // If File Was Input
                    // Change The Submit Day Of that Group
                    if (GetFileTxtPathWindow(filePath))
                    {
                        Date *today = new Date(27, 11, 2023);
                        projects[0]->addSubmissionDate(1, today);
                        std::stringstream group1SubmitDateText;
                        group1SubmitDateText << today->getDay() << "/" << today->getMonth() << "/" << today->getYear();

                        submissionStatusTexture.LoadFromRenderedText("Submission Status: " + group1SubmitDateText.str(), textFont, {255, 0, 255, 255});
                        submissionStatusTexture.SetPosition(64, 422);

                        // Turn Off the Submit Button
                        isSubmit = true;
                    }
#endif
                }
            }
        }
        // Delay Some Microsecond
        SDL_Delay(delay_ms);
    }

    return pageStatus;
}

#endif