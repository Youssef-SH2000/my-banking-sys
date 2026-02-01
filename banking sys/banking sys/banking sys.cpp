#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <iomanip>

using namespace std;

struct stClient {
    string Name;
    string AccountNumber;
    string PIN;
    string Phone;
    double AccountBalance = 0;
};
struct stUser {
    string Username;
    string Password;
    int Permissions;
};

enum enMainMenuOptions {
    eShowClientsList = 1,
    eAddClients = 2,
    eDeleteClient = 3,
    eUpdateClientInfo = 4,
    eFindClient = 5,
    eTransactions = 6,
    eManageUsers = 7,
    eLoggout = 8,
    eExit = 9
};
enum enTransactionsMenuOptions { eDeposit = 1,
    eWithdraw = 2,
    eTotalBalances = 3,
    eExitTranactions = 4
};
enum enPermissions {
    eAll = -1,
    pListClients = 1,
    pAddClient = 2,
    pDeleteClient = 4,
    pUpdateClient = 8,
    pFindClient = 16,
    pTransactions = 32,
    pManageUsers = 64
};
enum enManageUsersOptions {
    eShowUsersList = 1,
    eAddUser = 2,
    eDeleteUser = 3,
    eUpdateUserInfo = 4,
    eFindUser = 5,
    eMainMenu = 6
};

bool CheckAccessPermission(enPermissions Permission, int UserPermissions)
{
    if (UserPermissions == enPermissions::eAll)
        return true;

    if ((Permission & UserPermissions) == Permission)
        return true;

    return false;
}

void ShowAccessDeniedScreen()
{
    system("cls");
    cout << "\n===================================\n";
    cout << "        Access Denied!\n";
    cout << "===================================\n";
    cout << "\nYou don't have permission to perform this action.\n";
    cout << "Please contact your administrator.\n\n";
    cout << "Press any key to go back...";
    system("pause>0");
}

stClient ReadNewClient()
{
    stClient Client;
    cout << "Enter Account Number: ";
    getline(cin >> ws, Client.AccountNumber);
    cout << "Enter PinCode: ";
    getline(cin, Client.PIN);
    cout << "Enter Name: ";
    getline(cin, Client.Name);
    cout << "Enter Phone: ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance: ";
    cin >> Client.AccountBalance;
    return Client;
}

int ReadPermissionsToSet()
{
    int Permissions = 0;
    char Answer = 'n';

    cout << "\nDo you want to give full access? (y/n) ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        return -1;

    cout << "\nDo you want to give access to:\n";

    cout << "\nShow Clients List? (y/n) ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enPermissions::pListClients;

    cout << "Add New Client? (y/n) ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enPermissions::pAddClient;

    cout << "Delete Client? (y/n) ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enPermissions::pDeleteClient;

    cout << "Update Client Info? (y/n) ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enPermissions::pUpdateClient;

    cout << "Find Client? (y/n) ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enPermissions::pFindClient;

    cout << "Transactions? (y/n) ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enPermissions::pTransactions;

    cout << "Manage Users? (y/n) ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enPermissions::pManageUsers;

    return Permissions;
}

vector<string> SplitString(string String, string delimiter)
{
    vector<string> vResult;
    int pos;
    string sWord;
    while ((pos = String.find(delimiter)) != std::string::npos)
    {
        sWord = String.substr(0, pos);
        if (sWord != "")
            vResult.push_back(sWord);
        String.erase(0, pos + delimiter.length());
    }
    if (String != "")
        vResult.push_back(String);
    return vResult;
}

string ConvertClientRecordToLine(stClient& Client, string seperator = "#//#")
{
    string result = "";
    result += Client.AccountNumber + seperator;
    result += Client.PIN + seperator;
    result += Client.Name + seperator;
    result += Client.Phone + seperator;
    result += to_string(Client.AccountBalance);
    return result;
}

stClient ConvertLineToClientRecord(string LineData, string seperator = "#//#")
{
    stClient Client;
    vector<string> vClientData = SplitString(LineData, seperator);
    Client.AccountNumber = vClientData[0];
    Client.PIN = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);
    return Client;
}

vector<stClient> LoadClientDataFromFileToVector(string ClientsFileName)
{
    vector<stClient> vClients;
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            vClients.push_back(ConvertLineToClientRecord(Line));
        }
        MyFile.close();
    }
    return vClients;
}

void PrintClientRecord(stClient& Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PIN;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector<stClient>& vClients)
{
    cout << "\n\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n________________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n________________________________________________________________________________________________\n\n";
    for (stClient Client : vClients)
    {
        PrintClientRecord(Client);
        cout << '\n';
    }
    cout << "\n________________________________________________________________________________________________\n\n";
}

void ShowClientsScreen(string ClientsFileName, stUser User)
{
    if (CheckAccessPermission(enPermissions::pListClients, User.Permissions))
    {
        system("cls");
        vector<stClient> vClients = LoadClientDataFromFileToVector(ClientsFileName);
        PrintAllClientsData(vClients);
        cout << "\n\nPress any key to go back to main meune....";
        system("pause>0");
    }
    else
    {
        ShowAccessDeniedScreen();
    }
}

void PrintClientCard(stClient& Client)
{
    cout << "\nThe followings are the client data\n";
    cout << "----------------------------------\n";
    cout << "Account Number:  " << Client.AccountNumber << '\n';
    cout << "PIN code:        " << Client.PIN << '\n';
    cout << "Name:            " << Client.Name << '\n';
    cout << "Phone:           " << Client.Phone << '\n';
    cout << "Account Balance: " << Client.AccountBalance << '\n';
    cout << "----------------------------------\n\n";
}

bool FindClientByAccountNumber(string AccountNumber, string ClientsFileName, stClient& Client)
{
    vector<stClient> vClients = LoadClientDataFromFileToVector(ClientsFileName);
    for (stClient c : vClients)
    {
        if (c.AccountNumber == AccountNumber)
        {
            Client = c;
            return true;
        }
    }
    return false;
}

void WriteVectorContentInFile(string ClientsFileName, vector<string>& vFileContect)
{
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::out);

    if (MyFile.is_open())
    {
        for (string& line : vFileContect)
        {
            if (line != "")
            {
                MyFile << line << '\n';
            }
        }
        MyFile.close();
    }
}

vector<string> LoadFileTextToVector(string ClientsFileName)
{
    vector<string> vText;
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            vText.push_back(Line);
        }
        MyFile.close();
    }
    return vText;
}

void DeleteLineFromFile(string FileName, string Line)
{
    vector<string> vFileContent = LoadFileTextToVector(FileName);
    for (string& line : vFileContent)
    {
        if (line == Line)
            line = "";
    }
    WriteVectorContentInFile(FileName, vFileContent);
}

void UpdateLineFromFile(string FileName, string OldLine, string NewLine)
{
    vector<string> vFileContent = LoadFileTextToVector(FileName);
    for (string& line : vFileContent)
    {
        if (line == OldLine)
            line = NewLine;
    }
    WriteVectorContentInFile(FileName, vFileContent);
}

void ShowMainMenuScreen()
{
    cout << "==========================================\n";
    cout << "              Main Menu                \n";
    cout << "==========================================\n";
    cout << "\t[1] Show Clients List.\n";
    cout << "\t[2] Add Clients.\n";
    cout << "\t[3] Delete Clients.\n";
    cout << "\t[4] Update Client INFO.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Loggout.\n";
    cout << "\t[9] Exit.\n";
    cout << "==========================================\n";
    cout << "Choose what do you want to do (1-9)? ";
}

void AddClientDataLineToFile(string ClientsFileName)
{
    cout << "Adding new client\n\n";

    stClient Client;
    cout << "Enter Account Number: ";
    getline(cin >> ws, Client.AccountNumber);
    while (FindClientByAccountNumber(Client.AccountNumber, ClientsFileName, Client))
    {
        cout << "Client with account number (" << Client.AccountNumber << ") already exists, enter another account number: ";
        getline(cin >> ws, Client.AccountNumber);
    }
    cout << "Enter PinCode: ";
    getline(cin, Client.PIN);
    cout << "Enter Name: ";
    getline(cin, Client.Name);
    cout << "Enter Phone: ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance: ";
    cin >> Client.AccountBalance;
    string Record = ConvertClientRecordToLine(Client);

    fstream MyFile;
    MyFile.open(ClientsFileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << Record << '\n';
        MyFile.close();
    }
    cout << "\nClient added successfully! \n\n";
}

void AddClients(string ClientsFileName)
{
    char addMore = 'Y';
    do
    {
        AddClientDataLineToFile(ClientsFileName);
        cout << "Do you want to add more clients type [Y/N]? ";
        cin >> addMore;
    } while (toupper(addMore) == 'Y');
}

void AddNewClientsScreen(string ClientsFileName, stUser User)
{
    if (CheckAccessPermission(enPermissions::pAddClient, User.Permissions))
    {
        system("cls");
        cout << "\n--------------------------------------\n";
        cout << "        Add new clients screen\n";
        cout << "--------------------------------------\n";
        AddClients(ClientsFileName);
        cout << "\n\nPress any key to go back to main meune....";
        system("pause>0");
    }
    else
    {
        ShowAccessDeniedScreen();
    }
}

void DeleteClientScreen(string ClientsFileName, stUser User)
{
    if (CheckAccessPermission(enPermissions::pDeleteClient, User.Permissions))
    {
        system("cls");
        stClient Client;
        char Delete = 'N';
        cout << "\n----------------------------------------\n";
        cout << "\tDelete Client Screen\n";
        cout << "----------------------------------------\n";
        cout << "\nPlease enter account number: ";
        cin >> Client.AccountNumber;
        if (!FindClientByAccountNumber(Client.AccountNumber, ClientsFileName, Client))
        {
            cout << "\nClient with account number [" << Client.AccountNumber << "] is not found!\n\n";
            cout << "Press any key to go back to main Menu....";
            system("pause>0");
        }
        else
        {
            PrintClientCard(Client);
            cout << "Are you sure you want to delete this client (y/n)? ";
            cin >> Delete;
            if (toupper(Delete) == 'Y')
            {
                string Line = ConvertClientRecordToLine(Client);
                DeleteLineFromFile(ClientsFileName, Line);
                cout << "\nClient deleted successfully!\n\n";
                cout << "Press any key to go back to main Menu....";
                system("pause>0");
            }
            else
            {
                cout << "\nDeletion canceled!\n\n";
                cout << "Press any key to go back to main Menu....";
                system("pause>0");
            }
        }
    }
    else
    {
        ShowAccessDeniedScreen();
    }
    
}

void UpdateClientInfoScreen(string ClientsFileName, stUser User)
{
    if (CheckAccessPermission(enPermissions::pUpdateClient, User.Permissions))
    {
        system("cls");
        stClient Client;
        char Update = 'N';
        cout << "\n----------------------------------------\n";
        cout << "\tUpdate Client Screen\n";
        cout << "----------------------------------------\n";
        cout << "\nPlease enter account number: ";
        cin >> Client.AccountNumber;
        if (!FindClientByAccountNumber(Client.AccountNumber, ClientsFileName, Client))
        {
            cout << "\nClient with account number [" << Client.AccountNumber << "] is not found!\n\n";
            cout << "Press any key to go back to main Menu....";
            system("pause>0");
        }
        else
        {
            PrintClientCard(Client);
            cout << "Are you sure you want to update this client (y/n)? ";
            cin >> Update;
            if (toupper(Update) == 'Y')
            {
                string OldLine = ConvertClientRecordToLine(Client);
                cout << "PIN code:        ";
                getline(cin >> ws, Client.PIN);
                cout << "Name:            ";
                getline(cin, Client.Name);
                cout << "Phone:           ";
                getline(cin, Client.Phone);
                cout << "Account Balance: ";
                cin >> Client.AccountBalance;
                string NewLine = ConvertClientRecordToLine(Client);
                UpdateLineFromFile(ClientsFileName, OldLine, NewLine);
                cout << "\nClient updated successfully!\n\n";
                cout << "Press any key to go back to main Menu....";
                system("pause>0");
            }
            else
            {
                cout << "\nUpdating canceled!\n\n";
                cout << "Press any key to go back to main Menu....";
                system("pause>0");
            }
        }
    }
    else
    {
        ShowAccessDeniedScreen();
    }
    
}

void FindClientScreen(string ClientsFileName, stUser User)
{
    if (CheckAccessPermission(enPermissions::pFindClient, User.Permissions))
    {
        system("cls");
        stClient Client;
        cout << "\n----------------------------------------\n";
        cout << "\tFind Client Screen\n";
        cout << "----------------------------------------\n";
        cout << "\nPlease enter account number: ";
        cin >> Client.AccountNumber;
        if (!FindClientByAccountNumber(Client.AccountNumber, ClientsFileName, Client))
        {
            cout << "\nClient with account number [" << Client.AccountNumber << "] is not found!\n\n";
            cout << "Press any key to go back to main Menu....";
            system("pause>0");
        }
        else
        {
            PrintClientCard(Client);
            cout << "Press any key to go back to main Menu....";
            system("pause>0");
        }
    }
    else
    {
        ShowAccessDeniedScreen();
    }
}

void ShowTransactionsMenuScreen()
{
    cout << "==========================================\n";
    cout << "         Transactions Screen            \n";
    cout << "==========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total balances.\n";
    cout << "\t[4] Go back to main Menu.\n";
    cout << "==========================================\n";
    cout << "Choose what do you want to do (1-4)? ";
}

void ShowDepositScreen(string ClientsFileName)
{
    stClient Client;
    int dAmount = 0;
    char confirm = 'N';
    system("cls");
    cout << "\n-----------------------------------";
    cout << "\n\tDeposit Screen\n";
    cout << "-----------------------------------\n\n";
    cout << "Please enter account number: ";
    cin >> Client.AccountNumber;
    while (!FindClientByAccountNumber(Client.AccountNumber, ClientsFileName, Client))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] doesn't exist.\n\n";
        cout << "Please enter account number: ";
        cin >> Client.AccountNumber;
    }
    PrintClientCard(Client);
    cout << "\nPlease enter deposit amount: ";
    cin >> dAmount;
    cout << "\nAre you sure you want to preform this transaction (y/n)? ";
    cin >> confirm;
    string OldClient = ConvertClientRecordToLine(Client);
    if (toupper(confirm) == 'Y')
    {
        Client.AccountBalance += dAmount;
        string NewClient = ConvertClientRecordToLine(Client);
        UpdateLineFromFile(ClientsFileName, OldClient, NewClient);
        cout << "\nTransaction done successfully!\n";
        cout << "\nPress any key to go back to main Menu...";
        system("pause>0");
    }
    else
    {
        cout << "\nTransaction canceled!\n";
        cout << "\nPress any key to go back to main Menu...";
        system("pause>0");
    }
}

void ShowWithDrawScreen(string ClientsFileName)
{
    system("cls");
    stClient Client;
    char Confirm = 'N';
    int WithdrawAmount = 0;
    cout << "\n----------------------------------------\n";
    cout << "\tWithdraw Screen\n";
    cout << "----------------------------------------\n";
    cout << "\nPlease enter account number: ";
    cin >> Client.AccountNumber;
    if (!FindClientByAccountNumber(Client.AccountNumber, ClientsFileName, Client))
    {
        cout << "\nClient with account number [" << Client.AccountNumber << "] is not found!\n\n";
        cout << "Press any key to go back to main Menu....";
        system("pause>0");
    }
    else
    {
        PrintClientCard(Client);
        cout << "Please enter withdraw amount: ";
        cin >> WithdrawAmount;
        while (WithdrawAmount > Client.AccountBalance)
        {
            cout << "Amount exceeds the balance, you can withdraw up to: " << Client.AccountBalance << '\n';
            cout << "\nPlease enter another amount: ";
            cin >> WithdrawAmount;
        }
        cout << "Are you sure you want to confirm this transaction (y/n)? ";
        cin >> Confirm;
        if (toupper(Confirm) == 'Y')
        {
            string OldClient = ConvertClientRecordToLine(Client);
            Client.AccountBalance -= WithdrawAmount;
            string NewClient = ConvertClientRecordToLine(Client);
            UpdateLineFromFile(ClientsFileName, OldClient, NewClient);
            cout << "Transaction done successfully!\n\n";
            cout << "Press any key to go back to main menu...";
            system("pause>0");
        }
        else
        {
            cout << "Transaction canceled!\n\n";
            cout << "Press any key to go back to main menu...";
            system("pause>0");
        }
    }
}

void ShowTotalBalancesScreen(string ClientsFileName)
{
    system("cls");
    vector<stClient> vClients = LoadClientDataFromFileToVector(ClientsFileName);
    int TotalBalances = 0;
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n________________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n________________________________________________________________________________________________\n\n";
    for (stClient Client : vClients)
    {
        cout << "| " << setw(15) << left << Client.AccountNumber;
        cout << "| " << setw(40) << left << Client.Name;
        cout << "| " << setw(12) << left << Client.AccountBalance;
        TotalBalances += Client.AccountBalance;
        cout << '\n';
    }
    cout << "\n________________________________________________________________________________________________\n\n";
    cout << "\t\t\t\tTotal balances = " << TotalBalances << "\n\n";
    cout << "\n\nPress any key to go back to main meune....";
    system("pause>0");
}

void TransactionsScreen(string ClientsFileName, stUser User)
{
    if (CheckAccessPermission(enPermissions::pTransactions, User.Permissions))
    {
        short choice = 0;

        do
        {
            system("cls");
            ShowTransactionsMenuScreen();
            cin >> choice;

            switch (enTransactionsMenuOptions(choice))
            {
            case enTransactionsMenuOptions::eDeposit:
                ShowDepositScreen(ClientsFileName);
                break;

            case enTransactionsMenuOptions::eWithdraw:
                ShowWithDrawScreen(ClientsFileName);
                break;

            case enTransactionsMenuOptions::eTotalBalances:
                ShowTotalBalancesScreen(ClientsFileName);
                break;

            case enTransactionsMenuOptions::eExitTranactions:
                break;

            default:
                cout << "\nInvalid option! Please choose 1-4.\n";
                system("pause>0");
                break;
            }

        } while (enTransactionsMenuOptions(choice) != enTransactionsMenuOptions::eExitTranactions);
    }
    else
    {
        ShowAccessDeniedScreen();
    }

}

stUser ConvertLineToUserRecord(string LineData, string seperator = "#//#")
{
    stUser User;
    vector<string> vUserData = SplitString(LineData, seperator);
    User.Username = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);
    return User;
}

vector<stUser> LoadUserDataFromFileToVector(string UsersFileName)
{
    vector<stUser> vUsers;
    fstream MyFile;
    MyFile.open(UsersFileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            vUsers.push_back(ConvertLineToUserRecord(Line));
        }
        MyFile.close();
    }
    return vUsers;
}

bool FindUserByUsernameAndPassword(string UsersFileName, stUser &User)
{
    vector<stUser> vUsers = LoadUserDataFromFileToVector(UsersFileName);

    for (stUser u : vUsers)
    {
        if (User.Username == u.Username && User.Password == u.Password)
        {
            User = u;
            return true;
        }
    }
    return false;
}

bool FindUserByUsername(string UsersFileName, string Username, stUser &User)
{
    vector<stUser> vUsers = LoadUserDataFromFileToVector(UsersFileName);

    for (stUser u : vUsers)
    {
        if (Username == u.Username)
        {
            User = u;
            return true;
        }
    }
    return false;
}

stUser LogginScreen(string UsersFileName)
{
    system("cls");
    cout << "\n-------------------------------\n";
    cout << "\tLogin Screen\n";
    cout << "-------------------------------\n\n";

    stUser User;

    cout << "Enter Username: ";
    getline(cin, User.Username);
    cout << "Enter Password: ";
    getline(cin, User.Password);

    while (!FindUserByUsernameAndPassword(UsersFileName, User))
    {
        cout << "Invalid username/password\n";
        cout << "Enter Username: ";
        getline(cin, User.Username);
        cout << "Enter Password: ";
        getline(cin, User.Password);
    }
    return User;
}

void PrintManageUsersScreen()
{
    cout << "==========================================\n";
    cout << "           Manage Users Screen            \n";
    cout << "==========================================\n";
    cout << "\t[1] Show Users List.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menu.\n";
    cout << "==========================================\n";
    cout << "Choose what do you want to do (1-6)? ";
}

void PrintUserRecord(stUser& User)
{
    cout << "| " << setw(15) << left << User.Username;
    cout << "| " << setw(10) << left << User.Password;
    cout << "| " << setw(40) << left << User.Permissions;
}

void PrintAllUsersData(vector<stUser>& vUsers)
{
    cout << "\n\t\t\t\Users List (" << vUsers.size() << ") User(s).";
    cout << "\n________________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(15) << "Username";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n________________________________________________________________________________________________\n\n";
    for (stUser User : vUsers)
    {
        PrintUserRecord(User);
        cout << '\n';
    }
    cout << "\n________________________________________________________________________________________________\n\n";
}

void ShowUsersList(string UsersFileName)
{
    system("cls");
    vector<stUser> vUsers = LoadUserDataFromFileToVector(UsersFileName);
    PrintAllUsersData(vUsers);
    cout << "\n\nPress any key to go back to main meune....";
    system("pause>0");
}

string ConvertUserRecordToLine(stUser &User, string Seperator = "#//#")
{
    string result = "";
    result += User.Username + "#//#";
    result += User.Password + "#//#";
    result += to_string(User.Permissions);
    return result;
}

void PrintUserCard(stUser& User)
{
    cout << "\nThe followings are the user data\n";
    cout << "----------------------------------\n";
    cout << "Username   : " << User.Username << '\n';
    cout << "Password   : " << User.Password << '\n';
    cout << "Permissions: " << User.Permissions << '\n';
    cout << "----------------------------------\n\n";
}

void AddNewUser(string UsersFileName)
{
    system("cls");
    cout << "\n--------------------------------\n";
    cout << "\tAdd New User\n";
    cout << "--------------------------------\n";

    char AddMore = 'Y';

    do
    {
        cout << "Adding new user\n\n";

        stUser User;
        cout << "Enter Username: ";
        getline(cin >> ws, User.Username);
        while (FindUserByUsername(UsersFileName, User.Username, User))
        {
            cout << "User with [" << User.Username << "] already exists, enter another username? ";
            getline(cin >> ws, User.Username);
        }
        cout << "Enter Password: ";
        getline(cin, User.Password);
        
        User.Permissions = ReadPermissionsToSet();

        string UserLine = ConvertUserRecordToLine(User);

        fstream MyFile;
        MyFile.open(UsersFileName, ios::out | ios::app);
        if (MyFile.is_open())
        {
            MyFile << UserLine << '\n';
            MyFile.close();
        }
        cout << "\nClient added successfully! ,do you want to add more users (y/n)? ";
        cin >> AddMore;
        
    } while (toupper(AddMore) == 'Y');
}

void DeleteUser(string UsersFileName)
{
    system("cls");
    cout << "\n--------------------------------\n";
    cout << "\tDelete User\n";
    cout << "--------------------------------\n\n";

    stUser User;
    cout << "Enter Username: ";
    getline(cin >> ws, User.Username);
    if (!FindUserByUsername(UsersFileName, User.Username, User))
    {
        cout << "User with [" << User.Username << "] doesn't exists! \n\n";
        cout << "Press any key to go back to main menu...";
        system("pause>0");
        return;
    }
    if (User.Username == "Admin")
    {
        cout << "You cann't delete this user!\n\n";
        cout << "Press any key to go back to main menu...";
        system("pause>0");
        return;
    }
    char confirm = 'n';
    PrintUserCard(User);
    cout << "Are you sure you want to delete this user? (y/n) : ";
    cin >> confirm;
    if (toupper(confirm) == 'Y')
    {
        string Line = ConvertUserRecordToLine(User);
        DeleteLineFromFile(UsersFileName, Line);
        cout << "\nUser deleted successfully!\n\n";
        cout << "Press any key to go back to main Menu....";
        system("pause>0");
    }
    else
    {
        cout << "\nDeletion canceled!\n\n";
        cout << "Press any key to go back to main Menu....";
        system("pause>0");
    }
}

void UpdateUser(string UsersFileName)
{
    system("cls");
    cout << "\n--------------------------------\n";
    cout << "\Update User\n";
    cout << "--------------------------------\n\n";

    stUser User;
    cout << "Enter Username: ";
    getline(cin >> ws, User.Username);
    if (!FindUserByUsername(UsersFileName, User.Username, User))
    {
        cout << "User with [" << User.Username << "] doesn't exists! \n\n";
        cout << "Press any key to go back to main menu...";
        system("pause>0");
        return;
    }
    if (User.Username == "Admin")
    {
        cout << "You cann't update this user!\n\n";
        cout << "Press any key to go back to main menu...";
        system("pause>0");
        return;
    }
    char confirm = 'n';
    PrintUserCard(User);
    cout << "Are you sure you want to update this user? (y/n) : ";
    cin >> confirm;
    if (toupper(confirm) == 'Y')
    {
        string OldUserLine = ConvertUserRecordToLine(User);
        cout << "Enter Password? ";
        cin >> User.Password;
        User.Permissions = ReadPermissionsToSet();

        string NewUserLine = ConvertUserRecordToLine(User);

        UpdateLineFromFile(UsersFileName, OldUserLine, NewUserLine);

        cout << "\nUser updated successfully!\n\n";
        cout << "Press any key to go back to main Menu....";
        system("pause>0");

    }
    else
    {
        cout << "\Updating canceled!\n\n";
        cout << "Press any key to go back to main Menu....";
        system("pause>0");
    }
}

void FindUser(string UsersFileName)
{
    system("cls");
    cout << "\n--------------------------------\n";
    cout << "\Find User\n";
    cout << "--------------------------------\n\n";

    stUser User;
    cout << "Enter Username: ";
    getline(cin >> ws, User.Username);
    if (!FindUserByUsername(UsersFileName, User.Username, User))
    {
        cout << "User with [" << User.Username << "] doesn't exists! \n\n";
        cout << "Press any key to go back to main menu...";
        system("pause>0");
        return;
    }
    else
    {
        PrintUserCard(User);
        cout << "\nPress any key to go back to main menu...";
        system("pause>0");
    }
}

void ManageUsersScreen(string UsersFileName, stUser &User)
{
    if (CheckAccessPermission(enPermissions::pManageUsers, User.Permissions))
    {
        short choice = 0;

        do
        {
            system("cls");
            PrintManageUsersScreen();
            cin >> choice;

            switch (enManageUsersOptions(choice))
            {
            case enManageUsersOptions::eShowUsersList:
                ShowUsersList(UsersFileName);
                break;

            case enManageUsersOptions::eAddUser:
                AddNewUser(UsersFileName);
                break;

            case enManageUsersOptions::eDeleteUser:
                DeleteUser(UsersFileName);
                break;

            case enManageUsersOptions::eUpdateUserInfo:
                UpdateUser(UsersFileName);
                break;

            case enManageUsersOptions::eFindUser:
                FindUser(UsersFileName);
                break;

            case enManageUsersOptions::eMainMenu:
                break;

            default:
                cout << "\nInvalid option! Please choose 1-6.\n";
                system("pause>0");
                break;
            }

        } while (enMainMenuOptions(choice) != enMainMenuOptions::eExit);
    }
    else
    {
        ShowAccessDeniedScreen();
    }
}

void BankSys(string ClientsFileName, string UsersFileName)
{
    short choice = 0;
    stUser User = LogginScreen(UsersFileName);
    do
    {
        system("cls");
        ShowMainMenuScreen();
        cin >> choice;

        switch (enMainMenuOptions(choice))
        {
        case enMainMenuOptions::eShowClientsList:
            ShowClientsScreen(ClientsFileName, User);
            break;

        case enMainMenuOptions::eAddClients:
            AddNewClientsScreen(ClientsFileName, User);
            break;

        case enMainMenuOptions::eDeleteClient:
            DeleteClientScreen(ClientsFileName, User);
            break;

        case enMainMenuOptions::eUpdateClientInfo:
            UpdateClientInfoScreen(ClientsFileName, User);
            break;

        case enMainMenuOptions::eFindClient:
            FindClientScreen(ClientsFileName, User);
            break;

        case enMainMenuOptions::eTransactions:
            TransactionsScreen(ClientsFileName, User);
            break;

        case enMainMenuOptions::eManageUsers:
            ManageUsersScreen(UsersFileName, User);
            break;
        case enMainMenuOptions::eLoggout:
            BankSys(ClientsFileName, UsersFileName);
            break;
        case enMainMenuOptions::eExit:
            cout << "\nExiting program...\n";
            break;

        default:
            cout << "\nInvalid option! Please choose 1-9.\n";
            system("pause>0");
            break;
        }

    } while (enMainMenuOptions(choice) != enMainMenuOptions::eExit);
}





int main()
{
    BankSys("Clients.txt", "Users.txt");






    system("pause>0");
    return 0;

}
