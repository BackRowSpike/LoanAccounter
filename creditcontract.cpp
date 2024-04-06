#include "creditcontract.h"

//constructors
CreditContract::CreditContract(QString bfn, QString bpn, BorrowerTypeEnum bt, int bcr, QDate fpd, QString cn,
                               QString an, float lv, CurrencyEnum c, float p, RepaymentModeEnum rm, int pc, LoanPurposeEnum lp, float pr1, float pr2)
{
    if (validBorrowerFullName(bfn)) {
        borrowerFullName = bfn;
    } else {
        throw ProcessingException("Invalid borrower full name in CreditContract::CreditContract(...)");
    }
    if (validBorrowerPassportNumber(bpn)) {
        borrowerPassportNumber = bpn;
    } else {
        throw ProcessingException("Invalid borrower passport number in CreditContract::CreditContract(...)");
    }
    borrowerType = bt;
    if (validBorrowerCreditRating(bcr)) {
        borrowerCreditRating = bcr;
    } else {
        throw ProcessingException("Invalid borrower credit rating in CreditContract::CreditContract(...)");
    }
    if(validFirstPaymentDateAccelerated(fpd)) {
        firstPaymentDate = fpd;
    } else {
        throw ProcessingException("Invalid first payment date in CreditContract::CreditContract(...)");
    }
    if (validContractNumber(cn)) {
        contractNumber = cn;
    } else {
        throw ProcessingException("Invalid contract number in CreditContract::CreditContract(...)");
    }
    if (validAccountNumber(an)) {
        accountNumber = an;
    } else {
        throw ProcessingException("Invalid account number in CreditContract::CreditContract(...)");
    }
    if (validLoanValue(lv)) {
        loanValue = lv;
    } else {
        throw ProcessingException("Invalid loan value in CreditContract::CreditContract(...)");
    }
    currency = c;
    if (validPercent(p)) {
        percent = p;
    } else {
        throw ProcessingException("Invalid percent in CreditContract::CreditContract(...)");
    }
    repaymentMode = rm;
    if (validPaymentsCount(pc)) {
        paymentsCount = pc;
    } else {
        throw ProcessingException("Invalid payments count in CreditContract::CreditContract(...)");
    }
    loanPurpose = lp;
    if(validPenaltyRate(pr1)) {
        penaltyRate = pr1;
    } else {
        throw ProcessingException("Invalid penalty rate in CreditContract::CreditContract(...)");
    }
    if(validPledgeRate(pr2)) {
        pledgeRate = pr2;
    } else {
        throw ProcessingException("Invalid pledge rate in CreditContract::CreditContract(...)");
    }
}

CreditContract* CreditContract::createRandomCreditContract()
{
    try {
        CreditContract* cc = new CreditContract();
        //borrowerFullName
        QString str;
        if ((QRandomGenerator::global()->generate())%2) {
            str = GenericParametrs::femaleLastNames.at((QRandomGenerator::global()->generate())%6) + " "
                        + GenericParametrs::femaleFirstNames.at((QRandomGenerator::global()->generate())%6) + " "
                        + GenericParametrs::femaleMiddleNames.at((QRandomGenerator::global()->generate())%6);
        } else {
            str = GenericParametrs::maleLastNames.at((QRandomGenerator::global()->generate())%6) + " "
                        + GenericParametrs::maleFirstNames.at((QRandomGenerator::global()->generate())%6) + " "
                        + GenericParametrs::maleMiddleNames.at((QRandomGenerator::global()->generate())%6);
        }
        cc->setBorrowerFullName(str);
        //borrowerPassportNumber
        str = QString::number((QRandomGenerator::global()->generate())%6+1);
        int i = (QRandomGenerator::global()->generate())%30+1;
        if (i < 10) {
            str.append("0" + QString::number(i));
        } else {
            str.append(QString::number(i));
        }
        i = (QRandomGenerator::global()->generate())%12+1;
        if (i < 10) {
            str.append("0" + QString::number(i));
        } else {
            str.append(QString::number(i));
        }
        i = ((QRandomGenerator::global()->generate())%56+50)%100;
        if (i < 10) {
            str.append("0" + QString::number(i));
        } else {
            str.append(QString::number(i));
        }
        switch ((QRandomGenerator::global()->generate())%7) {
        case 0: str.append("A");
            break;
        case 1: str.append("B");
            break;
        case 2: str.append("C");
            break;
        case 3: str.append("K");
            break;
        case 4: str.append("E");
            break;
        case 5: str.append("M");
            break;
        default: str.append("H");
            break;
        }
        str.append(QString::number((QRandomGenerator::global()->generate())%10));
        str.append(QString::number((QRandomGenerator::global()->generate())%10));
        str.append(QString::number((QRandomGenerator::global()->generate())%10));
        switch ((QRandomGenerator::global()->generate())%3) {
        case 0: str.append("PB");
            break;
        case 1: str.append("BA");
            break;
        default: str.append("BI");
            break;
        }
        str.append(QString::number((QRandomGenerator::global()->generate())%10));
        cc->setBorrowerPassportNumber(str);
        //borrowerType
        if ((QRandomGenerator::global()->generate())%2) {
            cc->setBorrowerType(BorrowerTypeEnum::Entity);
        } else {
            cc->setBorrowerType(BorrowerTypeEnum::PrivateEntity);
        }
        //borrowerCreditRating
        cc->setBorrowerCreditRating(350 + (QRandomGenerator::global()->generate())%301);
        //firstPaymentDate
        cc->setFirstPaymentDate(QDate::currentDate().addDays(1));
        //contractNumber
        str = QString::number((QRandomGenerator::global()->generate())%10);
        for (i = 0; i < 9; i++) {
            str.append(QString::number((QRandomGenerator::global()->generate())%10));
        }
        cc->setContractNumber(str);
        //accountNumber
        str = "BY";
        str.append(QString::number((QRandomGenerator::global()->generate())%10));
        str.append(QString::number((QRandomGenerator::global()->generate())%10));
        str.append(GenericParametrs::getBankIBANCode());
        for (i = 0; i < 20; i++) {
            str.append(QString::number((QRandomGenerator::global()->generate())%10));
        }
        cc->setAccountNumber(str);
        //loanValue
        cc->setLoanValue(5000+250*((QRandomGenerator::global()->generate())%91));
        //currency
        if((QRandomGenerator::global()->generate())%2) {
            cc->setCurrency(CurrencyEnum::USD);
        } else {
            cc->setCurrency(CurrencyEnum::BYN);
        }
        //percent
        cc->setPercent(11+0.05*((QRandomGenerator::global()->generate())%80));
        //repaymentMode
        if((QRandomGenerator::global()->generate())%2) {
            cc->setRepaymentMode(RepaymentModeEnum::DIFF);
        } else {
            cc->setRepaymentMode(RepaymentModeEnum::AN);
        }
        //paymentsCount
        cc->setPaymentsCount(6+((QRandomGenerator::global()->generate())%31));
        //purpose
        switch ((QRandomGenerator::global()->generate())%4) {
        case 0: cc->setLoanPurpose(LoanPurposeEnum::ReplenishingWorkingCapital);
            break;
        case 1: cc->setLoanPurpose(LoanPurposeEnum::Buyer);
            break;
        case 2: cc->setLoanPurpose(LoanPurposeEnum::RealProperty);
            break;
        default: cc->setLoanPurpose(LoanPurposeEnum::Ohter);
            break;
        }
        //penaltyRate
        cc->setPenaltyRate(0.1*(QRandomGenerator::global()->generate()%6));
        //pledgeRate
        cc->setPledgeRate(2.5*(QRandomGenerator::global()->generate()%9));
        return cc;
    } catch(const ProcessingException &e) {
        throw ProcessingException("{" + e.getMessage() + "} in CreditContract::createRandomCreditContract()");
    }
}

CreditContract::CreditContract(CreditContract &cc)
{
    try {
        borrowerFullName = cc.getBorrowerFullName();
        borrowerPassportNumber = cc.getBorrowerPassportNumber();
        borrowerType = cc.getBorrowerType();
        borrowerCreditRating = cc.getBorrowerCreditRating();
        firstPaymentDate = cc.getFirstPaymentDate();
        contractNumber = cc.getContractNumber();
        accountNumber = cc.getAccountNumber();
        loanValue = cc.getLoanValue();
        currency = cc.getCurrency();
        percent = cc.getPercent();
        repaymentMode = cc.getRepaymentMode();
        paymentsCount = cc.getPaymentsCount();
        loanPurpose = cc.getLoanPurpose();
        penaltyRate = cc.getPenaltyRate();
        pledgeRate = cc.getPledgeRate();
    } catch(const ProcessingException &e) {
        throw ProcessingException("{" + e.getMessage() + "} in CreditContract::CreditContract(CreditContract &cc)");
    }

}

CreditContract::CreditContract() {}

//validation
bool CreditContract::validBorrowerFullName(QString bfn)
{
    if (bfn.isEmpty()) return false;
    for(int i = 0; i < bfn.length(); i++) {
        if (!bfn.at(i).isLetter() && bfn.at(i) != ' ') {
            return false;
        }
    }
    return true;
}

bool CreditContract::validBorrowerPassportNumber(QString bpn)
{
    if (bpn.length() != 14) return false;
    static QRegularExpression regex("[123456]"
                             "((0[1-9])|([12][0-9])|(30))"
                             "((0[1-9])|(10|11|12))"
                             "(([5-9][0-9])|(0[0-5]))"
                             "[ABCKEMH]"
                             "([0-9]{3})(PB|BA|BI)[0-9]");
    if (!regex.match(bpn).hasMatch()) {
        return false;
    }
    return true;
}

bool CreditContract::validBorrowerCreditRating(int bcr)
{
    return (bcr >= 0 && bcr <= 999);
}

bool CreditContract::validFirstPaymentDate(QDate fpd)
{
    return (fpd > QDate::currentDate() && fpd <= QDate::currentDate().addMonths(1));
}

bool CreditContract::validFirstPaymentDateAccelerated(QDate fpd)
{
    return (fpd > QDate::currentDate());
}

bool CreditContract::validContractNumber(QString cn)
{
    if (cn.length() != 10) return false;
    for(int i = 0; i < cn.length(); i++) {
        if (!cn.at(i).isDigit()) {
            return false;
        }
    }
    return true;
}

bool CreditContract::validAccountNumber(QString an)
{
    if (an.length() != 28) return false;
    static QRegularExpression regex("BY([0-9]{2})" +
                             GenericParametrs::getBankIBANCode() +
                             "([0-9]{20})");
    if (!regex.match(an).hasMatch()) {
        return false;
    }
    return true;
}

bool CreditContract::validLoanValue(float lv)
{
    return (lv >= GenericParametrs::getMinLoanValue() && lv <= GenericParametrs::getMaxLoanValue());
}

bool CreditContract::validPercent(float p)
{
    return (p >= GenericParametrs::getOfficialBankRate() && p < GenericParametrs::getMaxPercent());
}

bool CreditContract::validPaymentsCount(int pc)
{
    return (pc >= 0 && pc <= 360);
}

bool CreditContract::validPenaltyRate(float pr)
{
    return (pr >= 0 && pr <= 100);
}

bool CreditContract::validPledgeRate(float p)
{
    return (p >= 0 && p <= 100);
}

//setters
void CreditContract::setBorrowerFullName(QString bfn)
{
    if(validBorrowerFullName(bfn)) {
        mutex.lock();
        borrowerFullName = bfn;
        mutex.unlock();
    }
    else {
        throw ProcessingException("Invalid borrower full name in CreditContract::setBorrowerFullName(QString bfn)");
    }
}

void CreditContract::setBorrowerPassportNumber(QString bpn)
{
    if(validBorrowerPassportNumber(bpn)) {
        mutex.lock();
        borrowerPassportNumber = bpn;
        mutex.unlock();
    }
    else {
        throw ProcessingException("Invalid borrower passport number in CreditContract::setBorrowerPassportNumber(QString bpn)");
    }
}

void CreditContract::setBorrowerType(BorrowerTypeEnum bt)
{
    mutex.lock();
    borrowerType = bt;
    mutex.unlock();
}

void CreditContract::setBorrowerCreditRating(int bcr)
{
    if(validBorrowerCreditRating(bcr)) {
        mutex.lock();
        borrowerCreditRating = bcr;
        mutex.unlock();
    }
    else {
        throw ProcessingException("Invalid borrower credit rating in CreditContract::setBorrowerCreditRating(int bcr)");
    }
}

void CreditContract::setFirstPaymentDate(QDate fpd)
{
    if(validFirstPaymentDate(fpd)) {
        mutex.lock();
        firstPaymentDate = fpd;
        mutex.unlock();
    }
    else {
        throw ProcessingException("Invalid first payment date in CreditContract::setFirstPaymentDate(QDate fpd)");
    }
}

void CreditContract::setFirstPaymentDateAccelerated(QDate fpda)
{
    if(validFirstPaymentDateAccelerated(fpda)) {
        mutex.lock();
        firstPaymentDate = fpda;
        mutex.unlock();
    }
    else {
        throw ProcessingException("Invalid first payment date in CreditContract::setFirstPaymentDateAccelerated(QDate fpda)");
    }
}

void CreditContract::setContractNumber(QString cn)
{
    if(validContractNumber(cn)) {
        mutex.lock();
        contractNumber = cn;
        mutex.unlock();
    }
    else {
        throw ProcessingException("Invalid contract number in CreditContract::setContractNumber(QString cn)");
    }
}

void CreditContract::setAccountNumber(QString an)
{
    if(validAccountNumber(an)) {
        mutex.lock();
        accountNumber = an;
        mutex.unlock();
    }
    else {
        throw ProcessingException("Invalid account number in CreditContract::setAccountNumber(QString an)");
    }
}

void CreditContract::setLoanValue(float lv)
{
    if(validLoanValue(lv)) {
        mutex.lock();
        loanValue = lv;
        mutex.unlock();
    }
    else {
        throw ProcessingException("Invalid loan value in CreditContract::setLoanValue(float lv)");
    }
}

void CreditContract::setCurrency(CurrencyEnum c)
{
    mutex.lock();
    currency = c;
    mutex.unlock();
}

void CreditContract::setPercent(float p)
{
    if(validPercent(p)) {
        mutex.lock();
        percent = p;
        mutex.unlock();
    }
    else {
        throw ProcessingException("Invalid percent in CreditContract::setPercent(float p)");
    }
}

void CreditContract::setRepaymentMode(RepaymentModeEnum rm)
{
    mutex.lock();
    repaymentMode = rm;
    mutex.unlock();
}

void CreditContract::setPaymentsCount(int pc)
{
    if(validPaymentsCount(pc)) {
        mutex.lock();
        paymentsCount = pc;
        mutex.unlock();
    }
    else {
        throw ProcessingException("Invalid payments count in CreditContract::setPaymentsCount(int pc)");
    }
}

void CreditContract::setLoanPurpose(LoanPurposeEnum p)
{
    mutex.lock();
    loanPurpose = p;
    mutex.unlock();
}

void CreditContract::setPenaltyRate(float pr)
{
    if(validPenaltyRate(pr)) {
        mutex.lock();
        penaltyRate = pr;
        mutex.unlock();
    }
    else {
        throw ProcessingException("Invalid penalty rate in CreditContract::setPenaltyRate(float pr)");
    }
}

void CreditContract::setPledgeRate(float pr)
{
    if(validPledgeRate(pr)) {
        mutex.lock();
        pledgeRate = pr;
        mutex.unlock();
    }
    else {
        throw ProcessingException("Invalid pledge rate in CreditContract::setPledgeRate(float pr)");
    }
}

//getters
QString CreditContract::getBorrowerFullName()
{
    mutex.lock();
    QString bfn = borrowerFullName;
    mutex.unlock();
    return bfn;
}

QString CreditContract::getBorrowerPassportNumber()
{
    mutex.lock();
    QString bpn = borrowerPassportNumber;
    mutex.unlock();
    return bpn;
}

BorrowerTypeEnum CreditContract::getBorrowerType()
{
    mutex.lock();
    BorrowerTypeEnum bt = borrowerType;
    mutex.unlock();
    return bt;
}

int CreditContract::getBorrowerCreditRating()
{
    mutex.lock();
    int bcr = borrowerCreditRating;
    mutex.unlock();
    return bcr;
}

QDate CreditContract::getFirstPaymentDate()
{
    mutex.lock();
    QDate fpd = firstPaymentDate;
    mutex.unlock();
    return fpd;
}

QString CreditContract::getContractNumber()
{
    mutex.lock();
    QString cn = contractNumber;
    mutex.unlock();
    return cn;
}

QString CreditContract::getAccountNumber()
{
    mutex.lock();
    QString an = accountNumber;
    mutex.unlock();
    return an;
}

float CreditContract::getLoanValue()
{
    mutex.lock();
    float lv = loanValue;
    mutex.unlock();
    return lv;
}

CurrencyEnum CreditContract::getCurrency()
{
    mutex.lock();
    CurrencyEnum c = currency;
    mutex.unlock();
    return c;
}

float CreditContract::getPercent()
{
    mutex.lock();
    float p = percent;
    mutex.unlock();
    return p;
}

RepaymentModeEnum CreditContract::getRepaymentMode()
{
    mutex.lock();
    RepaymentModeEnum rm = repaymentMode;
    mutex.unlock();
    return rm;
}

int CreditContract::getPaymentsCount()
{
    mutex.lock();
    int pc = paymentsCount;
    mutex.unlock();
    return pc;
}

LoanPurposeEnum CreditContract::getLoanPurpose()
{
    mutex.lock();
    LoanPurposeEnum lp = loanPurpose;
    mutex.unlock();
    return lp;
}

float CreditContract::getPenaltyRate()
{
    mutex.lock();
    float pr = penaltyRate;
    mutex.unlock();
    return pr;
}

float CreditContract::getPledgeRate()
{
    mutex.lock();
    float pr = pledgeRate;
    mutex.unlock();
    return pr;
}
