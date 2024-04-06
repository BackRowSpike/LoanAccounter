#include "genericparametrs.h"

GenericParametrs::GenericParametrs() {}

QList<QString> GenericParametrs::maleFirstNames = {"Михаил", "Артём", "Виктор", "Илья", "Роман", "Антон"};
QList<QString> GenericParametrs::maleMiddleNames = {"Евгеньевич", "Сергеевич", "Витальевич", "Валерьевич", "Иванович", "Алексеевич"};
QList<QString> GenericParametrs::maleLastNames = {"Иванов", "Зайцев", "Петров", "Сидоров", "Новиков", "Савицкий"};
QList<QString> GenericParametrs::femaleFirstNames = {"Мария", "Анна", "Светлана", "Юлия", "Ольга", "Елена"};
QList<QString> GenericParametrs::femaleMiddleNames = {"Сергеевна", "Александровна", "Николаевна", "Владимировна", "Андреевна", "Дмитриевна"};
QList<QString> GenericParametrs::femaleLastNames = {"Фомина", "Матвеева", "Осипова", "Беляева", "Краснова", "Мороз"};

float GenericParametrs::getOfficialBankRate()
{
    return 10.5;
}

float GenericParametrs::getMinLoanValue()
{
    return 1000;
}

float GenericParametrs::getMaxLoanValue()
{
    return 100000;
}

float GenericParametrs::getMaxPercent()
{
    return 30;
}

float GenericParametrs::getSpeedUp()
{
    return 86400;     //1 day in 3 seconds
}

QString GenericParametrs::getBankIBANCode()
{
    return "LOVE";
}

int GenericParametrs::randomGenerationFrequency()
{
    return 7;
}

QString GenericParametrs::currencyEnumToString(CurrencyEnum c)
{
    switch(c) {
    case CurrencyEnum::USD:
        return "USD";
    default:
        return "BYN";
    }
}

QString GenericParametrs::borrowerTypeEnumToString(BorrowerTypeEnum bt)
{
    switch(bt) {
    case BorrowerTypeEnum::Entity:
        return "entity";
    default:
        return "private entity";
    }
}

QString GenericParametrs::repaymentModeEnumToString(RepaymentModeEnum rm)
{
    switch(rm) {
    case RepaymentModeEnum::DIFF:
        return "DIFF";
    default:
        return "AN";
    }
}

QString GenericParametrs::loanPurposeEnumToString(LoanPurposeEnum lp)
{
    switch(lp) {
    case LoanPurposeEnum::Buyer:
        return "buyer";
    case LoanPurposeEnum::RealProperty:
        return "real property";
    case LoanPurposeEnum::ReplenishingWorkingCapital:
        return "replenishing working capital";
    default:
        return "other";
    }
}

CurrencyEnum GenericParametrs::currencyEnumFromString(QString c)
{
    if (c == "USD") {
        return CurrencyEnum::USD;
    } else if (c == "BYN") {
        return CurrencyEnum::BYN;
    } else {
        throw ProcessingException("Unkonw currency in GenericParametrs::currencyEnumFromString(QString c)");
    }
}

BorrowerTypeEnum GenericParametrs::borrowerTypeEnumFromString(QString bt)
{
    if (bt == "entity") {
        return BorrowerTypeEnum::Entity;
    } else if (bt == "private entity") {
        return BorrowerTypeEnum::PrivateEntity;
    } else {
        throw ProcessingException("Unkonw borrower type in GenericParametrs::borrowerTypeEnumFromString(QString bt)");
    }
}

RepaymentModeEnum GenericParametrs::repaymentModeEnumFromString(QString rm)
{
    if (rm == "DIFF") {
        return RepaymentModeEnum::DIFF;
    } else if (rm == "AN") {
        return RepaymentModeEnum::AN;
    } else {
        throw ProcessingException("Unkonw repayment mode in GenericParametrs::repaymentModeEnumFromString(QString rm)");
    }
}

LoanPurposeEnum GenericParametrs::loanPurposeEnumFromString(QString lp)
{
    if (lp == "buyer") {
        return LoanPurposeEnum::Buyer;
    } else if (lp == "real property") {
        return LoanPurposeEnum::RealProperty;
    } else if (lp == "replenishing working capital") {
        return LoanPurposeEnum::ReplenishingWorkingCapital;
    } else if (lp == "other") {
        return LoanPurposeEnum::Ohter;
    } else {
        throw ProcessingException("Unkonw loan purpose in GenericParametrs::loanPurposeEnumFromString(QString lp)");
    }
}
