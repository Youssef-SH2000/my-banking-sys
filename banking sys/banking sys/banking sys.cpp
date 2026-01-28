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

enum enMainMenueOptions { eShowClientsList = 1, eAddClients = 2, eDeleteClient = 3, eUpdateClientInfo = 4, eFindClient = 5, eTransactions = 6, eManageUsers = 7, eLoggout = 8, eExit = 9 };
enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eTotalBalances = 3, eExitTranactions = 4 };

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

string ConvertRecordToLine(stClient& Client, string seperator = "#//#")
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

void ShowClientsScreen(string ClientsFileName)
{
    system("cls");
    vector<stClient> vClients = LoadClientDataFromFileToVector(ClientsFileName);
    PrintAllClientsData(vClients);
    cout << "\n\nPress any key to go back to main meune....";
    system("pause>0");
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

void DeleteLineFromFile(string ClientsFileName, string Line)
{
    vector<string> vFileContent = LoadFileTextToVector(ClientsFileName);
    for (string& line : vFileContent)
    {
        if (line == Line)
            line = "";
    }
    WriteVectorContentInFile(ClientsFileName, vFileContent);
}

void UpdateLineFromFile(string ClientsFileName, string OldLine, string NewLine)
{
    vector<string> vFileContent = LoadFileTextToVector(ClientsFileName);
    for (string& line : vFileContent)
    {
        if (line == OldLine)
            line = NewLine;
    }
    WriteVectorContentInFile(ClientsFileName, vFileContent);
}

void ShowMainMenueScreen()
{
    cout << "==========================================\n";
    cout << "              Main menue                \n";
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
    string Record = ConvertRecordToLine(Client);

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

void AddNewClientsScreen(string ClientsFileName)
{
    system("cls");
    cout << "\n--------------------------------------\n";
    cout << "        Add new clients screen\n";
    cout << "--------------------------------------\n";
    AddClients(ClientsFileName);
    cout << "\n\nPress any key to go back to main meune....";
    system("pause>0");
}

void DeleteClientScreen(string ClientsFileName)
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
        cout << "Press any key to go back to main menue....";
        system("pause>0");
    }
    else
    {
        PrintClientCard(Client);
        cout << "Are you sure you want to delete this client (y/n)? ";
        cin >> Delete;
        if (toupper(Delete) == 'Y')
        {
            string Line = ConvertRecordToLine(Client);
            DeleteLineFromFile(ClientsFileName, Line);
            cout << "\nClient deleted successfully!\n\n";
            cout << "Press any key to go back to main menue....";
            system("pause>0");
        }
        else
        {
            cout << "\nDeletion canceled!\n\n";
            cout << "Press any key to go back to main menue....";
            system("pause>0");
        }
    }
}

void UpdateClientInfoScreen(string ClientsFileName)
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
        cout << "Press any key to go back to main menue....";
        system("pause>0");
    }
    else
    {
        PrintClientCard(Client);
        cout << "Are you sure you want to update this client (y/n)? ";
        cin >> Update;
        if (toupper(Update) == 'Y')
        {
            string OldLine = ConvertRecordToLine(Client);
            cout << "PIN code:        ";
            getline(cin >> ws, Client.PIN);
            cout << "Name:            ";
            getline(cin, Client.Name);
            cout << "Phone:           ";
            getline(cin, Client.Phone);
            cout << "Account Balance: ";
            cin >> Client.AccountBalance;
            string NewLine = ConvertRecordToLine(Client);
            UpdateLineFromFile(ClientsFileName, OldLine, NewLine);
            cout << "\nClient updated successfully!\n\n";
            cout << "Press any key to go back to main menue....";
            system("pause>0");
        }
        else
        {
            cout << "\nUpdating canceled!\n\n";
            cout << "Press any key to go back to main menue....";
            system("pause>0");
        }
    }
}

void FindClientScreen(string ClientsFileName)
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
        cout << "Press any key to go back to main menue....";
        system("pause>0");
    }
    else
    {
        PrintClientCard(Client);
        cout << "Press any key to go back to main menue....";
        system("pause>0");
    }
}

void ShowTransactionsMenueScreen()
{
    cout << "==========================================\n";
    cout << "         Transactions Screen            \n";
    cout << "==========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total balances.\n";
    cout << "\t[4] Go back to main menue.\n";
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
    string OldClient = ConvertRecordToLine(Client);
    if (toupper(confirm) == 'Y')
    {
        Client.AccountBalance += dAmount;
        string NewClient = ConvertRecordToLine(Client);
        UpdateLineFromFile(ClientsFileName, OldClient, NewClient);
        cout << "\nTransaction done successfully!\n";
        cout << "\nPress any key to go back to main menue...";
        system("pause>0");
    }
    else
    {
        cout << "\nTransaction canceled!\n";
        cout << "\nPress any key to go back to main menue...";
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
        cout << "Press any key to go back to main menue....";
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
            string OldClient = ConvertRecordToLine(Client);
            Client.AccountBalance -= WithdrawAmount;
            string NewClient = ConvertRecordToLine(Client);
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

void TransactionsScreen(string ClientsFileName)
{
    short choice = 0;

    do
    {
        system("cls");
        ShowTransactionsMenueScreen();
        cin >> choice;

        switch (enTransactionsMenueOptions(choice))
        {
        case enTransactionsMenueOptions::eDeposit:
            ShowDepositScreen(ClientsFileName);
            break;

        case enTransactionsMenueOptions::eWithdraw:
            ShowWithDrawScreen(ClientsFileName);
            break;

        case enTransactionsMenueOptions::eTotalBalances:
            ShowTotalBalancesScreen(ClientsFileName);
            break;

        case enTransactionsMenueOptions::eExitTranactions:
            break;

        default:
            cout << "\nInvalid option! Please choose 1-4.\n";
            system("pause>0");
            break;
        }

    } while (enTransactionsMenueOptions(choice) != enTransactionsMenueOptions::eExitTranactions);
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

bool FindUser(string UsersFileName, stUser User)
{
    vector<stUser> vUsers = LoadUserDataFromFileToVector(UsersFileName);

    for (int i = 0; i < vUsers.size(); i++)
    {
        if (User.Username == vUsers[i].Username && User.Password == vUsers[i].Password)
            return true;
    }
    return false;
}

void LogginScreen(string UsersFileName)
{
    system("cls");
    cout << "\n-------------------------------\n";
    cout << "\tLogin Screen\n";
    cout << "-------------------------------\n\n";

    vector<stUser> vUsers = LoadUserDataFromFileToVector(UsersFileName);

    stUser User;

    cout << "Enter Username: ";
    getline(cin, User.Username);
    cout << "Enter Password: ";
    getline(cin, User.Password);
    
    while (!FindUser(UsersFileName, User))
    {
        cout << "Invalid username/password\n";
        cout << "Enter Username: ";
        getline(cin, User.Username);
        cout << "Enter Password: ";
        getline(cin, User.Password);
    }
}

void BankSys(string ClientsFileName, string UsersFileName)
{
    short choice = 0;

    do
    {
        system("cls");
        ShowMainMenueScreen();
        cin >> choice;

        switch (enMainMenueOptions(choice))
        {
        case enMainMenueOptions::eShowClientsList:
            ShowClientsScreen(ClientsFileName);
            break;

        case enMainMenueOptions::eAddClients:
            AddNewClientsScreen(ClientsFileName);
            break;

        case enMainMenueOptions::eDeleteClient:
            DeleteClientScreen(ClientsFileName);
            break;

        case enMainMenueOptions::eUpdateClientInfo:
            UpdateClientInfoScreen(ClientsFileName);
            break;

        case enMainMenueOptions::eFindClient:
            FindClientScreen(ClientsFileName);
            break;

        case enMainMenueOptions::eTransactions:
            TransactionsScreen(ClientsFileName);
            break;

            case enMainMenueOptions::eManageUsers:
                ManageUsersScreen(ClientsFileName);
            break;
            case enMainMenueOptions::eLoggout:
                LogginScreen(ClientsFileName);
            break;
        case enMainMenueOptions::eExit:
            cout << "\nExiting program...\n";
            break;

        default:
            cout << "\nInvalid option! Please choose 1-7.\n";
            system("pause>0");
            break;
        }

    } while (enMainMenueOptions(choice) != enMainMenueOptions::eExit);
}





int main()
{
    LoginScreen("Users.txt");
    





    system("pause>0");
    return 0;

}
