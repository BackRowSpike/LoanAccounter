#ifndef GENERICPARAMETRS_H
#define GENERICPARAMETRS_H

#include <QString>
#include <QList>
#include "processingexception.h"

enum CurrencyEnum {BYN, USD};
enum BorrowerTypeEnum {Entity, PrivateEntity};
enum RepaymentModeEnum {AN, DIFF};
enum LoanPurposeEnum {Buyer, RealProperty, ReplenishingWorkingCapital, Ohter};

class GenericParametrs
{
public:
    GenericParametrs();
    static float getOfficialBankRate();
    static float getMinLoanValue();
    static float getMaxLoanValue();
    static float getMaxPercent();
    static float getSpeedUp();
    static QString getBankIBANCode();
    static QList<QString> maleFirstNames;
    static QList<QString> maleMiddleNames;
    static QList<QString> maleLastNames;
    static QList<QString> femaleFirstNames;
    static QList<QString> femaleMiddleNames;
    static QList<QString> femaleLastNames;
    static int randomGenerationFrequency();
    static QString currencyEnumToString(CurrencyEnum);
    static QString borrowerTypeEnumToString(BorrowerTypeEnum);
    static QString repaymentModeEnumToString(RepaymentModeEnum);
    static QString loanPurposeEnumToString(LoanPurposeEnum);
    static CurrencyEnum currencyEnumFromString(QString);
    static BorrowerTypeEnum borrowerTypeEnumFromString(QString);
    static RepaymentModeEnum repaymentModeEnumFromString(QString);
    static LoanPurposeEnum loanPurposeEnumFromString(QString);
};

#endif // GENERICPARAMETRS_H
