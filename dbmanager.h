#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "creditcontract.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

class DBManager
{
private:
    QMutex mutex;
    QSqlDatabase* db;
    QSqlQuery* query;
public:
    DBManager();
    ~DBManager();
    void addNewCreditContract(CreditContract*, bool bot = false);
    CreditContract* getCreditContract(QString);
    void changeCreditContract(CreditContract*);
    void deleteCreditContract(QString);
    void setNewTransaction(QString, float);
    float getAccountBalance(QString);
};

#endif // DBMANAGER_H
