#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <iomanip>

using namespace std;

struct sClient {
    string Name;
    string AccountNumber;
    string PIN;
    string Phone;
    double AccountBalance = 0;
};

enum enMainMenueOptions { eShowClientsList = 1, eAddClients = 2, eDeleteClient = 3, eUpdateClientInfo = 4, eFindClient = 5, eTransactions = 6, eExit = 7 };
enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eTotalBalances = 3, eExitTranactions = 4 };

sClient ReadNewClient()
{
    sClient Client;
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

string ConvertRecordToLine(sClient& Client, string seperator = "##")
{
    string result = "";
    result += Client.AccountNumber + seperator;
    result += Client.PIN + seperator;
    result += Client.Name + seperator;
    result += Client.Phone + seperator;
    result += to_string(Client.AccountBalance);
    return result;
}

sClient ConvertLineToRecord(string LineData, string seperator = "##")
{
    sClient Client;
    vector<string> vClientData = SplitString(LineData, seperator);
    Client.AccountNumber = vClientData[0];
    Client.PIN = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);
    return Client;
}

vector<sClient> LoadDataFromFileToVector(string FileName)
{
    vector<sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            vClients.push_back(ConvertLineToRecord(Line));
        }
        MyFile.close();
    }
    return vClients;
}

void PrintClientRecord(sClient& Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PIN;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector<sClient>& vClients)
{
    cout << "\n\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n________________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n________________________________________________________________________________________________\n\n";
    for (sClient Client : vClients)
    {
        PrintClientRecord(Client);
        cout << '\n';
    }
    cout << "\n________________________________________________________________________________________________\n\n";
}

void ShowClientsScreen(string FileName)
{
    system("cls");
    vector<sClient> vClients = LoadDataFromFileToVector(FileName);
    PrintAllClientsData(vClients);
    cout << "\n\nPress any key to go back to main meune....";
    system("pause>0");
}

void PrintClientCard(sClient& Client)
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

bool FindClientByAccountNumber(string AccountNumber, string FileName, sClient& Client)
{
    vector<sClient> vClients = LoadDataFromFileToVector(FileName);
    for (sClient c : vClients)
    {
        if (c.AccountNumber == AccountNumber)
        {
            Client = c;
            return true;
        }
    }
    return false;
}

void WriteVectorContentInFile(string FileName, vector<string>& vFileContect)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

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

vector<string> LoadFileTextToVector(string FileName)
{
    vector<string> vText;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

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
    cout << "\t[7] Exit.\n";
    cout << "==========================================\n";
    cout << "Choose what do you want to do (1-7)? ";
}

void AddClientDataLineToFile(string FileName)
{
    cout << "Adding new client\n\n";

    sClient Client;
    cout << "Enter Account Number: ";
    getline(cin >> ws, Client.AccountNumber);
    while (FindClientByAccountNumber(Client.AccountNumber, FileName, Client))
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
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << Record << '\n';
        MyFile.close();
    }
    cout << "\nClient added successfully! \n\n";
}

void AddClients(string FileName)
{
    char addMore = 'Y';
    do
    {
        AddClientDataLineToFile(FileName);
        cout << "Do you want to add more clients type [Y/N]? ";
        cin >> addMore;
    } while (toupper(addMore) == 'Y');
}

void AddNewClientsScreen(string FileName)
{
    system("cls");
    cout << "\n--------------------------------------\n";
    cout << "        Add new clients screen\n";
    cout << "--------------------------------------\n";
    AddClients(FileName);
    cout << "\n\nPress any key to go back to main meune....";
    system("pause>0");
}

void DeleteClientScreen(string FileName)
{
    system("cls");
    sClient Client;
    char Delete = 'N';
    cout << "\n----------------------------------------\n";
    cout << "\tDelete Client Screen\n";
    cout << "----------------------------------------\n";
    cout << "\nPlease enter account number: ";
    cin >> Client.AccountNumber;
    if (!FindClientByAccountNumber(Client.AccountNumber, FileName, Client))
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
            DeleteLineFromFile(FileName, Line);
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

void UpdateClientInfoScreen(string FileName)
{
    system("cls");
    sClient Client;
    char Update = 'N';
    cout << "\n----------------------------------------\n";
    cout << "\tUpdate Client Screen\n";
    cout << "----------------------------------------\n";
    cout << "\nPlease enter account number: ";
    cin >> Client.AccountNumber;
    if (!FindClientByAccountNumber(Client.AccountNumber, FileName, Client))
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
            UpdateLineFromFile(FileName, OldLine, NewLine);
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

void FindClientScreen(string FileName)
{
    system("cls");
    sClient Client;
    cout << "\n----------------------------------------\n";
    cout << "\tFind Client Screen\n";
    cout << "----------------------------------------\n";
    cout << "\nPlease enter account number: ";
    cin >> Client.AccountNumber;
    if (!FindClientByAccountNumber(Client.AccountNumber, FileName, Client))
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

void ShowDepositScreen(string FileName)
{
    sClient Client;
    int dAmount = 0;
    char confirm = 'N';
    system("cls");
    cout << "\n-----------------------------------";
    cout << "\n\tDeposit Screen\n";
    cout << "-----------------------------------\n\n";
    cout << "Please enter account number: ";
    cin >> Client.AccountNumber;
    while (!FindClientByAccountNumber(Client.AccountNumber, FileName, Client))
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
        UpdateLineFromFile(FileName, OldClient, NewClient);
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

void ShowWithDrawScreen(string FileName)
{
    system("cls");
    sClient Client;
    char Confirm = 'N';
    int WithdrawAmount = 0;
    cout << "\n----------------------------------------\n";
    cout << "\tWithdraw Screen\n";
    cout << "----------------------------------------\n";
    cout << "\nPlease enter account number: ";
    cin >> Client.AccountNumber;
    if (!FindClientByAccountNumber(Client.AccountNumber, FileName, Client))
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
            UpdateLineFromFile(FileName, OldClient, NewClient);
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

void ShowTotalBalancesScreen(string FileName)
{
    system("cls");
    vector<sClient> vClients = LoadDataFromFileToVector(FileName);
    int TotalBalances = 0;
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n________________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n________________________________________________________________________________________________\n\n";
    for (sClient Client : vClients)
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

void TransactionsScreen(string FileName)
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
            ShowDepositScreen(FileName);
            break;

        case enTransactionsMenueOptions::eWithdraw:
            ShowWithDrawScreen(FileName);
            break;

        case enTransactionsMenueOptions::eTotalBalances:
            ShowTotalBalancesScreen(FileName);
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

void BankSys(string FileName)
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
            ShowClientsScreen(FileName);
            break;

        case enMainMenueOptions::eAddClients:
            AddNewClientsScreen(FileName);
            break;

        case enMainMenueOptions::eDeleteClient:
            DeleteClientScreen(FileName);
            break;

        case enMainMenueOptions::eUpdateClientInfo:
            UpdateClientInfoScreen(FileName);
            break;

        case enMainMenueOptions::eFindClient:
            FindClientScreen(FileName);
            break;
        case enMainMenueOptions::eTransactions:
            TransactionsScreen(FileName);
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
    BankSys("Clients.txt");





    system("pause>0");
    return 0;
}