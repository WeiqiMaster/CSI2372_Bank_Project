//************************************************************************
//
//   Project
//   Fall 2019
//
//   Mohamed TALEB
//
// This program reads information about clients and transactions in the following 2 files:
//          - clients.txt
//          - transact.txt
//
// It also allows certain operations on the read data:
//          - Display client bank accounts
//          - Updating client accounts
//
//************************************************************************
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iomanip>

#include "BankABC.h"

using namespace std;

double DepositAccount::rate = 6.50;
double totalAccounts = 0.0;

//******************************************************************
// Basic functions of the BankAccount class
//******************************************************************
inline BankAccount::BankAccount()
{
     accountId = type = updateDate = 0;
     clientName = new char[1];
     balance = 0.0;
}

inline BankAccount::BankAccount(long id, int newType,
                                char *name, long newDate, double newBalance) : accountId(id), type(newType),
                                                                               updateDate(newDate), balance(newBalance)
{
     clientName = new char[strlen(name) + 1];
     strcpy(clientName, name);
}

inline BankAccount::~BankAccount()
{
     if (clientName)
     {
          delete[] clientName;
     }
}

inline void BankAccount::setAccountId(long account)
{
     accountId = account;
}

inline void BankAccount::setType(int newType)
{
     type = newType;
}

inline void BankAccount::setClientName(char *name)
{
     if (clientName)
     {
          delete[] clientName;
     }
     clientName = new char[strlen(name) + 1];
     strcpy(clientName, name);
}

inline void BankAccount::setUpdateDate(long newDate)
{
     updateDate = newDate;
}

inline void BankAccount::setBalance(double newBalance)
{
     balance = newBalance;
}

void BankAccount::print()
{
     cout << endl;
     cout.setf(ios::fixed);
     cout.precision(2);
     cout << accountId << "\t\t\t" << type << "\t" << updateDate << "\t\t" << balance;
}

//******************************************************************
// Basic functions of the DepositAccount class
//******************************************************************
inline DepositAccount::DepositAccount(long id, int newType, char *name,
                                      long newDate, double balanceArg, int nbyear) : BankAccount(id, newType, name,
                                                                                                 newDate, balanceArg),
                                                                                     nbyears(nbyear)
{
}

inline void DepositAccount::setNbYears(int nbyear)
{
     nbyears = nbyear;
}

void DepositAccount::print()
{
     Bonus();
     BankAccount::print();
     cout.setf(ios::fixed);
     cout.precision(2);
     cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Functions of the class DepositAccount. It allows to calculate
// the annual bonus on deposit accounts.
// Inputs: Nothing
// Outputs: Nothing
//******************************************************************
void DepositAccount::Bonus()
{
     setBalance(getBalance() + (getBalance() * getRate() * (getNbYears()) / 36000.00));
}

//******************************************************************
// Basic functions of the LoanAccount class
//******************************************************************
inline LoanAccount::LoanAccount(long id, int newType, char *name,
                                long newDate, double newBalance, int nbyear,
                                double newRate) : BankAccount(id, newType,
                                                              name, newDate, newBalance),
                                                  nbyears(nbyear), rate(newRate)
{
}

inline void LoanAccount::setNbYears(int nbyear)
{
     nbyears = nbyear;
}

inline void LoanAccount::setRate(double newRate)
{
     rate = newRate;
}

void LoanAccount::print()
{
     BankAccount::print();
     cout.setf(ios::fixed);
     cout.precision(2);
     cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Basic functions of the Transaction class
//******************************************************************
inline Transaction::Transaction(long idTr, int typeTr, long dateTr,
                                int codeTr = 01, double amountTr) : accountId(idTr), type(typeTr),
                                                                    date(dateTr), code(codeTr),
                                                                    amount(amountTr)
{
}

inline void Transaction::setAccountId(long compteTr)
{
     accountId = compteTr;
}

inline void Transaction::setType(int typeTr)
{
     type = typeTr;
}

inline void Transaction::setDate(long dateTr)
{
     date = dateTr;
}

inline void Transaction::setCode(int codeTr)
{
     code = codeTr;
}

inline void Transaction::setAmount(double amountTr)
{
     amount = amountTr;
}

//****************************************************************************
// Purpose: Sort a list of bank accounts in ascending order of ids and types.
//
// WARNING: This sort leaves the last account (of id 0) of the list which has
// its position to assure the tests of end of list later !!!
//
// Inputs: listAccount(BankAccount *), a list of bank accounts.
// Outputs: listAccount(BankAccount *), sorted list of bank accounts.
//****************************************************************************
void sortAccounts(BankAccount **list)
{
     BankAccount *pAccount;
     for (int i = 0; list[i + 1]->getAccountId() != 0; i++)
     {
          for (int j = i + 1; j >= 1; j--)
          {
               if (list[j]->getAccountId() < list[j - 1]->getAccountId())
               {
                    pAccount = list[j];
                    list[j] = list[j - 1];
                    list[j - 1] = pAccount;
               }
               else if (list[j]->getAccountId() == list[j - 1]->getAccountId() && list[j]->getType() < list[j - 1]->getType())
               {
                    pAccount = list[j];
                    list[j] = list[j - 1];
                    list[j - 1] = pAccount;
               }
               else
               {
                    break;
               }
          }
     }
}

//******************************************************************
// Purpose: This function reads the file 'clients.txt' and builds
// an array containing the different bank accounts of customers.
//
// Inputs: Nothing
// Output: listAccount(type: BankAccount *), the list of bank
//         accounts read from the file 'clients.txt'.
//******************************************************************
BankAccount **readAccounts()
{
     ifstream inputFile("clients.txt"); // Opening the input file
     if (!inputFile)                    // If the file is not found...
     {
          cout << "File not found !!!" << endl;
          exit(0);
     }

     BankAccount **listAccounts = new BankAccount *[K_SizeMax];
     if (!listAccounts)
     {
          cout << "Full memory. !!!" << endl;
          return listAccounts;
     }

     BankAccount **pAccount = listAccounts;

     long accountRead, dateRead;
     int TypeRead, nbyearRead, counter = 0;
     double balanceRead, RateRead;
     char nameRead[60];

     inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
     inputFile.getline(nameRead, 60);

     while (inputFile && (counter < K_SizeMax - 1))
     {
          // YOU HAVE TO DO SOMETHING FROM HERE !!!

          if (TypeRead == 03)
          {
               *pAccount = new DepositAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead);
          }
          else
          {
               if (TypeRead == 04)
               {        
                    // double temp = (balanceRead * nbyearRead * RateRead) / 354445;
                    // cout << balanceRead << endl;
                    // cout << balanceRead + temp << endl;
                    balanceRead = balanceRead + ((balanceRead * nbyearRead * RateRead) / 36000);
                    cout << balanceRead<<endl;
                    *pAccount = new LoanAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead, RateRead);
               }
               else
               {
                    *pAccount = new BankAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead);
               }
          }


          // UNTIL THIS POINT.

          inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
          inputFile.getline(nameRead, 60);
          pAccount++;
          counter++;
     }
     *pAccount = new BankAccount();
     return listAccounts;
}

//*****************************************************************************************
// Purpose: This function validates whether the transaction code
//          corresponds to the correct account:
//              - 01 ==> account (01: Check, 02: Savings, 03: Deposit and 04: Loan)
//              - 02 ==> account (01: Check, 02: Savings)
//              - 03 ==> account (01: Check).
//
// Inputs: trans (Type: Transaction) an instance of the Transaction class.
// Outputs: true (Type bool) if the transaction matches one of the accounts listed above.
// false (Type bool) otherwise.
//*****************************************************************************************
Bool BankAccount::validateTransaction(const Transaction trans) const
{
     if (((trans.getCode() == 02) && (isDepositAccount() || isLoanAccount())) ||
         ((trans.getCode() == 03) && (isDepositAccount() || isLoanAccount() || isSavingsAccount())))
     {
          return FALSE;
     }
     else
     {
          return TRUE;
     }
}

//******************************************************************************
// Purpose: This function is used to execute the transaction already performed
// (update the balance of an account).
//
// Inputs: trans (Transaction Type), instance of Transaction class
// Outputs: Nothing
//*******************************************************************************
void BankAccount::executeTransaction(const Transaction trans)
{
     if (validateTransaction(trans))
     {
          if (trans.getCode() == 01) // Deposit
          {
               setBalance(getBalance() + trans.getAmount());
          }
          else
          {
               if (trans.getCode() == 02) // Withdrawal
               {
                    if (getBalance() >= trans.getAmount())
                    {
                         setBalance(getBalance() - (trans.getAmount() + 0.50));
                    }
                    else
                    {
                         cout << " insufficient balance!! " << endl;
                    }
               }
               else // Check
               {
                    if (getBalance() >= trans.getAmount())
                    {
                         setBalance(getBalance() - trans.getAmount());
                    }
                    else
                    {
                         cout << " insufficient balance!! " << endl;
                    }
               }
          }
     }
}

//***********************************************************************
// Purpose: This function is used to execute a read transaction
// (updating the balance of the loan account).
//
// Inputs: trans (Transaction Type), instance of the Transaction class
// Outputs: Nothing
//***********************************************************************
void LoanAccount::executeTransaction(const Transaction trans)
{
     if (validateTransaction(trans))
     {
          if (trans.getCode() == 01) // Deposit
          {
               setBalance(getBalance() - trans.getAmount());
          }
     }
}

//*************************************************************************
// Purpose: This function allows to read the file 'transact.txt'
//          and to update the accounts concerned by the transactions read.
//
// Inputs: listAccount (type BankAccount *), the list of bank accounts.
// Output: Nothing.
//*************************************************************************
void updateAccounts(BankAccount **listAccounts)
{

     ifstream inputFile("transact.txt"); // Opening the input file
     if (!inputFile)                     // If the file is not found...
     {
          cout << "File not found !!!" << endl;
          exit(0);
     }
     long accountRead, dateRead;
     int TypeRead, code;
     double amountRead;

     BankAccount **pAccount;
     while (inputFile)
     {
          pAccount = listAccounts;
          pAccount--;
          inputFile >> accountRead >> TypeRead >> dateRead >> code >> amountRead;
          Transaction trans(accountRead, TypeRead, dateRead,
                            code, amountRead);

          while (pAccount++ && (*pAccount)->getAccountId() != 0)
          {
               if ((*pAccount)->getAccountId() == accountRead && (*pAccount)->getType() == TypeRead)
               {
                    if ((*pAccount)->isLoanAccount())
                    {
                         LoanAccount *pB = dynamic_cast<LoanAccount *>(*pAccount);
                         pB->executeTransaction(trans);
                    }
                    else
                    {
                         (*pAccount)->executeTransaction(trans);
                    }

                    break;
               }
          }
     }
}

//******************************************************************************
// Purpose: This function displays the list of bank accounts for all customers.
//
// Inputs: listAccount (type: BankAccount *), the list of bank accounts.
// Outputs: Nothing
//******************************************************************************
void displayAccounts(BankAccount **listAccounts)
{
     cout << endl
          << endl
          << endl;

     Bool find[K_SizeMax];
     for (int k = 0; k < K_SizeMax; k++)
     {
          find[k] = FALSE;
     }

     cout << "                       THE REPORT OF THE BANK ACCOUNTS OF CLIENTS" << endl;
     cout << "                       ------------------------------------------" << endl
          << endl;
     BankAccount **pAccount = listAccounts;
     long accountRead, dateRead;
     int TypeRead, nbyearRead;
     double balanceRead, RateRead;
     // char *nameRead = new char[1];
     string nameRead;
     string nameReadPre;
     double total = 0.00;
     int flag = 0;
     do
     {
          accountRead = (*pAccount)->getAccountId();
          total += balanceRead;
          if (accountRead == 0)
          {
               if (flag != 0)
               {
                    cout << "\t\t\t\t\t\t\t\t   "
                         << "--------" << endl;
                    cout << "\t\t\t\t\tTOTAL ACCOUNTS: " << total << endl
                         << endl;
               }
               break;
          }

          nameRead = (*pAccount)->getClientName();
          balanceRead = (*pAccount)->getBalance();
          TypeRead = (*pAccount)->getType();
          dateRead = (*pAccount)->getUpdatedate();

          if (nameReadPre.compare(nameRead))
          {
               if (flag != 0)
               {
                    cout << "\t\t\t\t\t\t\t\t   "
                         << "--------" << endl;
                    cout << "\t\t\t\t\tTOTAL ACCOUNTS: " << total << endl
                         << endl;
                    total = 0.0;
               }
               cout << "\t Client Name: " << nameRead << endl
                    << endl;
               cout << "Bank Account \t Type  Update Date  Balance \t Nb.Years    Rate" << endl;
               cout << "------------ \t ----  -----------  ------- \t --------    ----" << endl;
               flag = 1;
          }

          if ((*pAccount)->isDepositAccount())
          {
               DepositAccount *pB = dynamic_cast<DepositAccount *>(*pAccount);
               pB->Bonus();
               balanceRead = pB->getBalance();
               nbyearRead = pB->getNbYears();
               RateRead = pB->getRate();
               cout << accountRead << "\t\t\t   " << TypeRead << "\t" << dateRead << "\t\t" << balanceRead << "    \t "
                    << nbyearRead << "   \t " << RateRead << endl;
          }
          else if ((*pAccount)->isLoanAccount())
          {
               LoanAccount *pB = dynamic_cast<LoanAccount *>(*pAccount);
               // DepositAccount *pB = dynamic_cast<DepositAccount *>(*pAccount);
               // pB->Bonus();
               nbyearRead = pB->getNbYears();
               RateRead = pB->getRate();
               cout << accountRead << "\t\t\t   " << TypeRead << "\t" << dateRead << "\t\t" << balanceRead << "    \t "
                    << nbyearRead << "  \t  " << RateRead << endl;
          }
          else
          {
               cout << accountRead << "\t\t\t   " << TypeRead << "\t" << dateRead << "\t\t" << balanceRead << "    \t " << endl;
          }

          nameReadPre = nameRead;
     } while (pAccount++);
}

int main()
{
     std::cout << std::setprecision(8);
     BankAccount **list = readAccounts();
     sortAccounts(list);
     // for (int i = 0; list[i] != NULL; i++)
     // {
     //      list[i]->print();
     // }
     displayAccounts(list);
     updateAccounts(list);
     cout << endl
          << endl;
     cout << "               ************************************************" << endl;
     cout << "               * REAFFICHAGE DES DONNEES APRES LA MISE A JOUR *" << endl;
     cout << "               ************************************************" << endl;
     displayAccounts(list);
     // cout << endl;

     // system("PAUSE");
     return 0;
}
