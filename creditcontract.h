#ifndef CREDITCONTRACT_H
#define CREDITCONTRACT_H

#include "genericparametrs.h"
#include "processingexception.h"
#include <QDate>
#include <QRegularExpression>
#include <QRandomGenerator>
#include <QMutex>

class CreditContract
{
private:
    QString borrowerFullName;
    QString borrowerPassportNumber;
    BorrowerTypeEnum borrowerType;
    int borrowerCreditRating;
    QDate firstPaymentDate;
    QString contractNumber;
    QString accountNumber;
    float loanValue;
    CurrencyEnum currency;
    float percent;
    RepaymentModeEnum repaymentMode;
    int paymentsCount;
    LoanPurposeEnum loanPurpose;
    float penaltyRate;
    float pledgeRate;
    QMutex mutex;
    CreditContract();
public:
    //constructors
    CreditContract(QString, QString, BorrowerTypeEnum, int, QDate, QString,
                   QString, float, CurrencyEnum, float, RepaymentModeEnum, int, LoanPurposeEnum, float, float);
    static CreditContract* createRandomCreditContract();
    CreditContract(CreditContract&);
    //validation
    static bool validBorrowerFullName(QString);
    static bool validBorrowerPassportNumber(QString);
    static bool validBorrowerCreditRating(int);
    static bool validFirstPaymentDate(QDate);
    static bool validFirstPaymentDateAccelerated(QDate);
    static bool validContractNumber(QString);
    static bool validAccountNumber(QString);
    static bool validLoanValue(float);
    static bool validPercent(float);
    static bool validPaymentsCount(int);
    static bool validPenaltyRate(float);
    static bool validPledgeRate(float);
    //setters
    void setBorrowerFullName(QString);
    void setBorrowerPassportNumber(QString);
    void setBorrowerType(BorrowerTypeEnum);
    void setBorrowerCreditRating(int);
    void setFirstPaymentDate(QDate);
    void setFirstPaymentDateAccelerated(QDate);
    void setContractNumber(QString);
    void setAccountNumber(QString);
    void setLoanValue(float);
    void setCurrency(CurrencyEnum);
    void setPercent(float);
    void setRepaymentMode(RepaymentModeEnum);
    void setPaymentsCount(int);
    void setLoanPurpose(LoanPurposeEnum);
    void setPenaltyRate(float);
    void setPledgeRate(float);
    //getters
    QString getBorrowerFullName();
    QString getBorrowerPassportNumber();
    BorrowerTypeEnum getBorrowerType();
    int getBorrowerCreditRating();
    QDate getFirstPaymentDate();
    QString getContractNumber();
    QString getAccountNumber();
    float getLoanValue();
    CurrencyEnum getCurrency();
    float getPercent();
    RepaymentModeEnum getRepaymentMode();
    int getPaymentsCount();
    LoanPurposeEnum getLoanPurpose();
    float getPenaltyRate();
    float getPledgeRate();
};

#endif // CREDITCONTRACT_H
