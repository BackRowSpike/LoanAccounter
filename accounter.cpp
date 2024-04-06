#include "accounter.h"

Accounter::Accounter(QObject *parent): QObject(parent)
{
    timerId = startTimer(86400000);
    currentDate = QDate::currentDate();
    daysInUse = 0;
    testMode = false;
    try {
        dbmanager = new DBManager();
    } catch(const ProcessingException &e) {
        throw ProcessingException("{" + e.getMessage() + "} in Accounter::Accounter()");
    }
}

Accounter::~Accounter()
{
    killTimer(timerId);
    delete dbmanager;
}

int Accounter::size()
{
    mutex.lock();
    int size = catalogue.size();
    mutex.unlock();
    return size;
}

CreditContract *Accounter::at(int i)
{
    if (i < 0 || i >= size()) {
        throw ProcessingException("Index out of range in Accounter::at(int i)");
    }
    mutex.lock();
    try {
        QMap<QString, CreditContractNote*>::iterator iter = catalogue.begin();
        std::advance(iter, i);
        CreditContract* cc = dbmanager->getCreditContract(iter.key());
        mutex.unlock();
        return cc;
    } catch (const ProcessingException& e) {
        mutex.unlock();
        throw ProcessingException("{" + e.getMessage() + "} in Accounter::at(int i)");
    }
}

void Accounter::insertInCatalogue(CreditContract* cc, bool bot)
{
    mutex.lock();
    if(catalogue.contains(cc->getContractNumber())) {
        mutex.unlock();
        throw ProcessingException("Contract number duplicate in Accounter::insertInCatalogue(CreditContract* cc)");
    }
    mutex.unlock();
    int PDC = cc->getFirstPaymentDate().toString("d").toInt();
    if (PDC > 28) {
        PDC = 1;
    }
    int daysBeforeFirstPayment = cc->getFirstPaymentDate().toJulianDay() - currentDate.toJulianDay();
    float payment, ELB0;
    switch(cc->getRepaymentMode()) {
        case RepaymentModeEnum::AN: {
            float coef = pow(0.01*cc->getPercent()+1, -1./12);
            ELB0 = cc->getLoanValue() * pow(0.01*cc->getPercent()+1, daysBeforeFirstPayment/365) * (1 - coef) * cc->getPaymentsCount() / (1 - pow(coef, cc->getPaymentsCount()));
            payment = ELB0/cc->getPaymentsCount();
            break;
        }
        default: {
            ELB0 = cc->getLoanValue() * pow(0.01*cc->getPercent()+1, daysBeforeFirstPayment/365) * (1 + cc->getPercent()*(cc->getPaymentsCount()+1)/2400);
            payment = 1.*cc->getLoanValue()/cc->getPaymentsCount();
            break;
        }
    }
    mutex.lock();
    catalogue.insert(cc->getContractNumber(), new CreditContractNote(PDC, ELB0, payment));
    if(cc->getPaymentsCount() == 0) {
        catalogue.find(cc->getContractNumber()).value()->underPayment = true;
    }
    mutex.unlock();
    try {
        dbmanager->addNewCreditContract(cc, bot);
        dbmanager->setNewTransaction(cc->getAccountNumber(), ELB0);
    } catch(const ProcessingException &e) {
        throw ProcessingException("{" + e.getMessage() + "} in Accounter::insertInCatalogue(CreditContract* cc)");
    }
    emit accounterChanged();
}

void Accounter::removeFromCatalogue(QString cn)
{
    mutex.lock();
    if(catalogue.contains(cn)) {
        catalogue.remove(cn);
        mutex.unlock();
    } else {
        mutex.unlock();
        throw ProcessingException("Unkown contract number in Accounter::removeFromCatalogue(QString cn)");
    }
    try {
        dbmanager->deleteCreditContract(cn);
    } catch(const ProcessingException &e) {
        throw ProcessingException("{" + e.getMessage() + "} in Accounter::removeFromCatalogue(QString cn)");
    }
    emit accounterChanged();
}

CreditContract *Accounter::findInCatalogue(QString cn)
{
    try {
        return dbmanager->getCreditContract(cn);
    } catch(const ProcessingException &e) {
        throw ProcessingException("{" + e.getMessage() + "} in Accounter::findInCatalogue(QString cn)");
    }
}

float Accounter::getAccountBalance(QString cn)
{
    try {
        return dbmanager->getAccountBalance(cn);
    } catch(const ProcessingException &e) {
        throw ProcessingException("{" + e.getMessage() + "} in Accounter::getAccountBalance(QString cn)");
    }
}

void Accounter::setNewTransaction(QString cn, float v)
{
    try {
        dbmanager->setNewTransaction(findInCatalogue(cn)->getAccountNumber(), v);
        emit accounterChanged();
    } catch(const ProcessingException &e) {
        throw ProcessingException("{" + e.getMessage() + "} in Accounter::setNewTransaction(QString cn, float v)");
    }
}

bool Accounter::hasUnderpayment(QString cn)
{
    mutex.lock();
    if (!catalogue.contains(cn)) {
        mutex.unlock();
        throw ProcessingException("Unknown contract number in Accounter::hasUnderpayment(QString cn)");
    }
    bool hasunderpayment = catalogue.find(cn).value()->underPayment;
    mutex.unlock();
    return hasunderpayment;
}

void Accounter::testModeStateChanged()
{
    killTimer(timerId);
    if(testMode) {
        timerId = startTimer(86400000);
        QMap<QString, CreditContractNote*>::iterator i;
        QList<QString> contractNumbersToRemove;
        mutex.lock();
        for (i = catalogue.begin(); i != catalogue.end(); i++) {
            if(i.value()->bot) {
                contractNumbersToRemove.append(i.key());
            }
        }
        mutex.unlock();
        for(int j = 0; j < contractNumbersToRemove.size(); j++) {
            removeFromCatalogue(contractNumbersToRemove.at(j));
        }
        emit accounterChanged();
    } else {
        timerId = startTimer(86400000/GenericParametrs::getSpeedUp());
    }
    testMode = !testMode;
}

void Accounter::addNewCreditContract(CreditContract* cc)
{
    try {
        insertInCatalogue(cc, false);
    } catch(const ProcessingException &e) {
        throw ProcessingException("{" + e.getMessage() + "} in Accounter::addNewCreditContract(CreditContract* cc)");
    }
}

void Accounter::deleteCreditContract(QString cn)
{
    try {
        removeFromCatalogue(cn);
    } catch(const ProcessingException &e) {
        throw ProcessingException("{" + e.getMessage() + "} in Accounter::deleteCreditContract(QString cn)");
    }
}

void Accounter::changeCreditContract(CreditContract* cc)
{
    mutex.lock();
    if(!catalogue.contains(cc->getContractNumber())) {
        mutex.unlock();
        throw ProcessingException("Unkown contract number in Accounter::changeCreditContract(CreditContract* cc)");
    }
    mutex.unlock();
    try {
        mutex.lock();
        CreditContractNote* note = catalogue.find(cc->getContractNumber()).value();
        mutex.unlock();
        int CurDate = currentDate.toString("d").toInt();
        int PaymDateCount = cc->getFirstPaymentDate().toString("d").toInt();
        if (PaymDateCount > 28) {
            PaymDateCount = 1;
        }
        int daysBeforeNextPayment = PaymDateCount - CurDate;
        if(daysBeforeNextPayment < 1) {
            daysBeforeNextPayment += 30;
        }
        float payment, ELB;
        switch(cc->getRepaymentMode()) {
            case RepaymentModeEnum::AN: {
                float coef = pow(0.01*cc->getPercent()+1, -1./12);
                ELB = cc->getLoanValue() * pow(0.01*cc->getPercent()+1, daysBeforeNextPayment/365) * (1 - coef) * cc->getPaymentsCount() / (1 - pow(coef, cc->getPaymentsCount()));
                payment = ELB/cc->getPaymentsCount();
                break;
            }
            default: {
                ELB = cc->getLoanValue() * pow(0.01*cc->getPercent()+1, daysBeforeNextPayment/365) * (1 + cc->getPercent()*(cc->getPaymentsCount()+1)/2400);
                payment = 1.*cc->getLoanValue()/cc->getPaymentsCount();
                break;
            }
        }
        mutex.lock();
        note->paymentDateCounter = PaymDateCount;
        note->expectedLoanBalance = ELB;
        note->freeParametr = payment;
        mutex.unlock();
        dbmanager->changeCreditContract(cc);
        dbmanager->setNewTransaction(cc->getAccountNumber(), ELB-dbmanager->getAccountBalance(cc->getAccountNumber()));
    } catch(const ProcessingException &e) {
        throw ProcessingException("{" + e.getMessage() + "} in Accounter::changeCreditContract(CreditContract* cc)");
    }
}

void Accounter::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == timerId) {
        try {
            currentDate = currentDate.addDays(1);
            emit dateIncrementation();
            int CD = currentDate.toString("d").toInt();
            float RLB;
            QMap<QString, CreditContractNote*>::iterator i;
            CreditContractNote* note;
            CreditContract* cc;
            mutex.lock();
            for (i = catalogue.begin(); i != catalogue.end(); i++) {
                note = i.value();
                cc = dbmanager->getCreditContract(i.key());
                RLB = dbmanager->getAccountBalance(cc->getAccountNumber());
                if(note->paymentDateCounter == CD) {
                    if(RLB < 1) {
                        emit loanRepayment(i.key());
                        mutex.unlock();
                        removeFromCatalogue(i.key());
                        return;
                    }
                    if(cc->getPaymentsCount() == 0) {
                        mutex.unlock();
                        return;
                    }
                    cc->setPaymentsCount(cc->getPaymentsCount() - 1);
                    dbmanager->changeCreditContract(cc);
                    switch(cc->getRepaymentMode()) {
                        case RepaymentModeEnum::AN: {
                            if(note->expectedLoanBalance < RLB) {
                                note->expectedLoanBalance *= 1 + 0.01*cc->getPenaltyRate();
                                note->freeParametr *= 1 + 0.01*cc->getPenaltyRate();
                                if(note->underPayment) {
                                    emit loanUnderpayment(i.key());
                                } else {
                                    note->underPayment = true;
                                }
                            } else {
                                note->underPayment = false;
                                if(note->expectedLoanBalance > RLB) {
                                   note->freeParametr *= RLB/note->expectedLoanBalance;
                                   note->expectedLoanBalance = RLB;
                                }
                            }
                            note->expectedLoanBalance -= note->freeParametr;
                            break;
                        }
                        default: {
                            if(note->expectedLoanBalance < RLB) {
                                note->expectedLoanBalance *= 1 + 0.01*cc->getPenaltyRate();
                                note->freeParametr *= 1 + 0.01*cc->getPenaltyRate();
                                if(note->underPayment) {
                                    emit loanUnderpayment(i.key());
                                } else {
                                    note->underPayment = true;
                                }
                            } else {
                                note->underPayment = false;
                                if(note->expectedLoanBalance > RLB) {
                                   note->freeParametr = RLB/((1 + cc->getPercent()*(cc->getPaymentsCount() + 2)/2400)*(cc->getPaymentsCount()+1));
                                   note->expectedLoanBalance = RLB;
                                }
                            }
                            note->expectedLoanBalance -= note->freeParametr * (1 + cc->getPercent()*(cc->getPaymentsCount() + 1)/1200);
                            break;
                        }
                    }
                }
                else {
                    if(note->expectedLoanBalance > RLB) {
                        note->underPayment = false;
                    }
                    //random bots payments
                    if(testMode && note->paymentDateCounter == (CD + 1)%30 && note->bot) {
                        RLB = -1. * note->freeParametr*(0.75 + 0.01*(QRandomGenerator::global()->generate()%50));
                        dbmanager->setNewTransaction((dbmanager->getCreditContract(i.key()))->getAccountNumber(), RLB);
                    }
                }
            }
            mutex.unlock();
            daysInUse++;
            emit accounterChanged();
            //random bots generation
            if (testMode && daysInUse%GenericParametrs::randomGenerationFrequency() == 0) {
                CreditContract* bot = CreditContract::createRandomCreditContract();
                bot->setFirstPaymentDateAccelerated(currentDate.addDays(1));
                insertInCatalogue(bot, true);
                mutex.lock();
                catalogue.find(bot->getContractNumber()).value()->bot = true;
                mutex.unlock();
            }
        } catch(const ProcessingException &e) {
            throw ProcessingException("{" + e.getMessage() + "} in Accounter::timerEvent(QTimerEvent *e)");
        }
    }
}
