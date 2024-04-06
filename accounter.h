#ifndef ACCOUNTER_H
#define ACCOUNTER_H

#include "dbmanager.h"
#include <QObject>
#include <QTime>
#include <QTimer>
#include <QTimerEvent>
#include <QtMath>
#include <algorithm>

struct CreditContractNote {
    int paymentDateCounter;
    float expectedLoanBalance;
    float freeParametr;
    bool underPayment = false;
    bool bot = false;

    CreditContractNote(int pdc, float elb, float fp) {
        expectedLoanBalance = elb;
        paymentDateCounter = pdc;
        freeParametr = fp;
    }
};

class Accounter: public QObject
{
    Q_OBJECT
private:
    int timerId;
    QMap<QString, CreditContractNote*> catalogue;
    DBManager* dbmanager;
    QMutex mutex;
    QDate currentDate;
    int daysInUse;
    bool testMode;
protected:
    void timerEvent(QTimerEvent* e) override;
signals:
    void loanUnderpayment(QString);
    void accounterChanged();
    void loanRepayment(QString);
    void dateIncrementation();
public:
    Accounter(QObject *parent = 0);
    ~Accounter();
    int size();
    CreditContract* at(int);
    void insertInCatalogue(CreditContract*, bool bot = false);
    void removeFromCatalogue(QString);
    CreditContract* findInCatalogue(QString);
    float getAccountBalance(QString);
    void setNewTransaction(QString, float);
    bool hasUnderpayment(QString);
    void testModeStateChanged();
public slots:
    void addNewCreditContract(CreditContract*);
    void deleteCreditContract(QString);
    void changeCreditContract(CreditContract*);
};

#endif // ACCOUNTER_H
