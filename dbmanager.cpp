#include "dbmanager.h"

DBManager::DBManager()
{
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName("../LoanAccounter/source/LoanAccounterDatabase.db");

    if (!db->open()) {
        throw ProcessingException("Unable to connect a database in DBManager::DBManager()");
    }

    query = new QSqlQuery();
    query->exec("DROP TABLE CreditContract");
    if (!query->exec("CREATE TABLE CreditContract ("
             "borrowerFullName nvarchar(50) NOT NULL,"
             "borrowerPassportNumber nvarchar(14) NOT NULL,"
             "borrowerType nvarchar(20) NOT NULL,"
             "borrowerCreditRating int NOT NULL,"
             "firstPaymentDate date NOT NULL,"
             "contractNumber nvarchar(10) NOT NULL PRIMARY KEY,"
             "accountNumber nvarchar(28) NOT NULL,"
             "loanValue float NOT NULL,"
             "currency nvarchar(3) NOT NULL,"
             "[percent] float NOT NULL,"
             "repaymentMode nvarchar(4) NOT NULL,"
             "paymentsCount int NOT NULL,"
             "loanPurpose nvarchar(30) NOT NULL,"
             "penaltyRate float NOT NULL,"
             "pledgeRate float NOT NULL, "
             "bot nvarchar(5) NOT NULL)")) {
        throw ProcessingException("Unable to create CreditContract table in a database in DBManager::DBManager()");
    }
    query->exec("DROP TABLE Account");
    if (!query->exec("CREATE TABLE Account ("
             "accountNumber nvarchar(28) NOT NULL PRIMARY KEY,"
             "loanBalance float NOT NULL)")) {
        throw ProcessingException("Unable to create Account table in a database in DBManager::DBManager()");
    }
}

DBManager::~DBManager()
{
    db->close();
    delete query;
    delete db;
}

void DBManager::changeCreditContract(CreditContract * cc)
{
    mutex.lock();
    query->exec("SELECT COUNT(*) FROM CreditContract "
                        "WHERE contractNumber = '" + cc->getContractNumber() + "'");
    query->next();
    if(query->value(0).toInt() == 0) {
        mutex.unlock();
        throw ProcessingException("Unkown contract number in DBManager::changeCreditContract(CreditContract * cc)");
    }
    if(!query->exec("UPDATE CreditContract "
                    "SET borrowerFullName = '" + cc->getBorrowerFullName() + "', "
                    "borrowerType = '" + GenericParametrs::borrowerTypeEnumToString(cc->getBorrowerType()) + "', "
                    "borrowerCreditRating = " + QString::number(cc->getBorrowerCreditRating()) + ", "
                    "firstPaymentDate = '" + cc->getFirstPaymentDate().toString("yyyy-MM-dd") + "', "
                    "loanValue = " + QString::number(cc->getLoanValue()) + ", "
                    "[percent] = " + QString::number(cc->getPercent()) + ", "
                    "repaymentMode = '" + GenericParametrs::repaymentModeEnumToString(cc->getRepaymentMode()) + "', "
                    "paymentsCount = " + QString::number(cc->getPaymentsCount()) + ", "
                    "loanPurpose = '" + GenericParametrs::loanPurposeEnumToString(cc->getLoanPurpose()) + "', "
                    "penaltyRate = " + QString::number(cc->getPenaltyRate()) + ", "
                    "pledgeRate = " + QString::number(cc->getPledgeRate()) + " "
                    "WHERE contractNumber = '" + cc->getContractNumber() + "'")) {
        mutex.unlock();
        throw ProcessingException("Unable to change item in CreditContract table in DBManager::changeCreditContract(CreditContract * cc)");
    }
    mutex.unlock();
}

void DBManager::addNewCreditContract(CreditContract* cc, bool bot)
{
    mutex.lock();
    query->exec("SELECT COUNT(*) FROM CreditContract "
                        "WHERE contractNumber = '" + cc->getContractNumber() + "'");
    query->next();
    if(query->value(0).toInt() != 0) {
        mutex.unlock();
        throw ProcessingException("Duplicate contract number in DBManager::addNewCreditContract(CreditContract* cc, bool bot)");
    }
    query->exec("SELECT COUNT(*) FROM Account "
                        "WHERE accountNumber = '" + cc->getAccountNumber() + "'");
    query->next();
    if(query->value(0).toInt() != 0) {
        mutex.unlock();
        throw ProcessingException("Duplicate account number in DBManager::addNewCreditContract(CreditContract* cc, bool bot)");
    }
    if(!query->exec("INSERT INTO CreditContract VALUES('" +
                    cc->getBorrowerFullName() + "', '" +
                    cc->getBorrowerPassportNumber() + "', '" +
                    GenericParametrs::borrowerTypeEnumToString(cc->getBorrowerType()) + "', " +
                    QString::number(cc->getBorrowerCreditRating()) + ", '" +
                    cc->getFirstPaymentDate().toString("yyyy-MM-dd") + "', '" +
                    cc->getContractNumber() + "', '" +
                    cc->getAccountNumber() + "', " +
                    QString::number(cc->getLoanValue()) + ", '" +
                    GenericParametrs::currencyEnumToString(cc->getCurrency()) + "', " +
                    QString::number(cc->getPercent()) + ", '" +
                    GenericParametrs::repaymentModeEnumToString(cc->getRepaymentMode()) + "', " +
                    QString::number(cc->getPaymentsCount()) + ", '" +
                    GenericParametrs::loanPurposeEnumToString(cc->getLoanPurpose()) + "', " +
                    QString::number(cc->getPenaltyRate()) + ", " +
                    QString::number(cc->getPledgeRate()) + ", '" +
                    QVariant(bot).toString() + "')")) {
        mutex.unlock();
        throw ProcessingException("Unable to add new item to CreditContract table in DBManager::addNewCreditContract(CreditContract* cc, bool bot)");
    }
    if(!query->exec("INSERT  INTO Account VALUES('" +
                    cc->getAccountNumber() + "', 0)")) {
        mutex.unlock();
        throw ProcessingException("Unable to add new item to Account table in DBManager::addNewCreditContract(CreditContract* cc, bool bot)");
    }
    mutex.unlock();
}

CreditContract *DBManager::getCreditContract(QString cn)
{
    mutex.lock();
    query->exec("SELECT COUNT(*) FROM CreditContract "
                        "WHERE contractNumber = '" + cn + "'");
    query->next();
    if(query->value(0).toInt() == 0) {
        mutex.unlock();
        throw ProcessingException("Unkown contract number in DBManager::getCreditContract(QString cn)");
    }
    try {
        query->exec("SELECT * FROM CreditContract "
                            "WHERE contractNumber = '" + cn + "'");
        query->next();
        CreditContract* cc = new CreditContract(query->value(0).toString(),
            query->value(1).toString(),
            GenericParametrs::borrowerTypeEnumFromString(query->value(2).toString()),
            query->value(3).toInt(),
            query->value(4).toDate(),
            cn,
            query->value(6).toString(),
            query->value(7).toFloat(),
            GenericParametrs::currencyEnumFromString(query->value(8).toString()),
            query->value(9).toFloat(),
            GenericParametrs::repaymentModeEnumFromString(query->value(10).toString()),
            query->value(11).toInt(),
            GenericParametrs::loanPurposeEnumFromString(query->value(12).toString()),
            query->value(13).toFloat(),
            query->value(14).toFloat());
        mutex.unlock();
        return cc;
    } catch(const ProcessingException& e) {
        mutex.unlock();
        throw ProcessingException("{" + e.getMessage() + "} in DBManager::getCreditContract(QString cn)");
    }
}

void DBManager::deleteCreditContract(QString cn)
{
    mutex.lock();
    query->exec("SELECT COUNT(*) FROM CreditContract "
                        "WHERE contractNumber = '" + cn + "'");
    query->next();
    if(query->value(0).toInt() == 0) {
        mutex.unlock();
        throw ProcessingException("Unkown contract number in DBManager::deleteCreditContract(QString cn)");
    }
    if(!query->exec("DELETE FROM Account "
                    "WHERE Account.accountNumber IN ("
                    "SELECT CreditContract.accountNumber FROM CreditContract "
                    "WHERE contractNumber = '" + cn + "')")) {
        mutex.unlock();
        throw ProcessingException("Unable to delete item from Account table in DBManager::deleteCreditContract(QString cn)");
    }
    if(!query->exec("DELETE FROM CreditContract "
                    "WHERE contractNumber = '" + cn + "'")) {
        mutex.unlock();
        throw ProcessingException("Unable to delete item from CreditContract table in DBManager::deleteCreditContract(QString cn)");
    }
    mutex.unlock();
}

void DBManager::setNewTransaction(QString an, float v)
{
    mutex.lock();
    query->exec("SELECT COUNT(*) FROM Account "
                        "WHERE accountNumber = '" + an + "'");
    query->next();
    if(query->value(0).toInt() == 0) {
        mutex.unlock();
        throw ProcessingException("Unkown account number in DBManager::setNewTransaction(QString an, float v)");
    }
    if(!query->exec("UPDATE Account "
                    "SET loanBalance = [loanBalance] + (" + QString::number(v) + ") "
                    "WHERE accountNumber = '" + an + "'")) {
        mutex.unlock();
        throw ProcessingException("Unable to set new transaction in Account table in DBManager::setNewTransaction(QString an, float v)");
    }
    mutex.unlock();
}

float DBManager::getAccountBalance(QString an)
{
    mutex.lock();
    query->exec("SELECT COUNT(*) FROM Account "
                        "WHERE accountNumber = '" + an + "'");
    query->next();
    if(query->value(0).toInt() == 0) {
        mutex.unlock();
        throw ProcessingException("Unkown account number in DBManager::getAccountBalance(QString an)");
    }
    if(!query->exec("SELECT loanBalance FROM Account "
                    "WHERE accountNumber = '" + an + "'")) {
        mutex.unlock();
        throw ProcessingException("Unable to get loan balance in Account table in DBManager::getAccountBalance(...)");
    }
    query->next();
    float lb = query->value(0).toFloat();
    mutex.unlock();
    return lb;
}
