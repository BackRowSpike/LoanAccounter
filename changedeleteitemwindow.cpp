#include "changedeleteitemwindow.h"

ChangeDeleteItemWindow::ChangeDeleteItemWindow(QWidget *parent): AddNewChangeDeleteItemWindow::AddNewChangeDeleteItemWindow(parent)
{
    deleteItem = new QPushButton();
    deleteItem->setText("Delete from catalogue");
    QVBoxLayout *deleteItemLayout = new QVBoxLayout;
    deleteItemLayout->addWidget(deleteItem);
    QGroupBox* deleteItemGroupBox = new QGroupBox();
    deleteItemGroupBox->setLayout(deleteItemLayout);
    mainLayout->addWidget(deleteItemGroupBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ChangeDeleteItemWindow::acceptDialog);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ChangeDeleteItemWindow::rejectDialog);
    connect(deleteItem, &QPushButton::clicked,
                this, &ChangeDeleteItemWindow::deleteItemClicked);
    item = nullptr;
    borrowerPassportNumber->setReadOnly(true);
    contractNumber->setReadOnly(true);
    accountNumber->setReadOnly(true);
    currency->setDisabled(true);
    setWindowTitle(tr("Change/delete credit contract"));
}

void ChangeDeleteItemWindow::execute(CreditContract* cc, QDate curDate)
{
    borrowerFullName->setText(cc->getBorrowerFullName());
    borrowerPassportNumber->setText(cc->getBorrowerPassportNumber());
    switch(cc->getBorrowerType()) {
    case BorrowerTypeEnum::Entity:
        borrowerType->setCurrentIndex(1);
        break;
    default:
        borrowerType->setCurrentIndex(0);
        break;
    }
    borrowerCreditRating->setValue(cc->getBorrowerCreditRating());
    firstPaymentDate->setText(cc->getFirstPaymentDate().toString("dd-MM-yyyy"));
    contractNumber->setText(cc->getContractNumber());
    accountNumber->setText(cc->getAccountNumber());
    loanValue->setText(QString::number((double)cc->getLoanValue(), 'f', 2));
    switch(cc->getCurrency()) {
    case CurrencyEnum::USD:
        currency->setCurrentIndex(1);
        break;
    default:
        currency->setCurrentIndex(0);
        break;
    }
    percent->setValue(cc->getPercent());
    switch(cc->getRepaymentMode()) {
    case RepaymentModeEnum::DIFF:
        repaymentMode->setCurrentIndex(1);
        break;
    default:
        repaymentMode->setCurrentIndex(0);
        break;
    }
    paymentsCount->setValue(cc->getPaymentsCount());
    switch(cc->getLoanPurpose()) {
    case LoanPurposeEnum::Buyer:
        loanPurpose->setCurrentIndex(0);
        break;
    case LoanPurposeEnum::RealProperty:
        loanPurpose->setCurrentIndex(1);
        break;
    case LoanPurposeEnum::ReplenishingWorkingCapital:
        loanPurpose->setCurrentIndex(2);
        break;
    default:
        loanPurpose->setCurrentIndex(3);
        break;
    }
    penaltyRate->setValue(cc->getPenaltyRate());
    pledgeRate->setValue(cc->getPledgeRate());
    if(cc->getFirstPaymentDate() <= curDate) {
        firstPaymentDate->setReadOnly(true);
    }
    item = cc;
    this->exec();
}

void ChangeDeleteItemWindow::deleteItemClicked()
{
    if(item != nullptr) {
        emit creditContractDeleted(item->getContractNumber());
        item = nullptr;
    }
    accept();
}

void ChangeDeleteItemWindow::acceptDialog()
{
    if(item == nullptr) {
        accept();
    }
    else try {
        if (item->getContractNumber() == contractNumber->text() && atributesValidation()) {
            item->setBorrowerFullName(borrowerFullName->text());
            switch(borrowerType->currentIndex()) {
                case 1:
                    item->setBorrowerType(BorrowerTypeEnum::Entity);
                    break;
                default:
                    item->setBorrowerType(BorrowerTypeEnum::PrivateEntity);
                    break;
            }
            item->setBorrowerCreditRating(borrowerCreditRating->value());
            if(!firstPaymentDate->isReadOnly()) {
                item->setFirstPaymentDate(QDate::fromString(firstPaymentDate->text(), "dd-MM-yyyy"));
            }
            item->setLoanValue(loanValue->text().toFloat());
            item->setPercent(percent->value());
            switch(repaymentMode->currentIndex()) {
            case 1:
                item->setRepaymentMode(RepaymentModeEnum::DIFF);
                break;
            default:
                item->setRepaymentMode(RepaymentModeEnum::AN);
                break;
            }
            item->setPaymentsCount(paymentsCount->value());
            switch(loanPurpose->currentIndex()) {
            case 0:
                item->setLoanPurpose(LoanPurposeEnum::Buyer);
                break;
            case 1:
                item->setLoanPurpose(LoanPurposeEnum::RealProperty);
                break;
            case 2:
                item->setLoanPurpose(LoanPurposeEnum::ReplenishingWorkingCapital);
                break;
            default:
                item->setLoanPurpose(LoanPurposeEnum::Ohter);
                break;
            }
            item->setPledgeRate(pledgeRate->value());
            item->setPenaltyRate(penaltyRate->value());
            emit creditContractChanged(item);
            item = nullptr;
            accept();
        }
    } catch(const ProcessingException &e) {
        throw ProcessingException("{" + e.getMessage() + "} in ChangeDeleteItemWindow::acceptDialog()");
    }
}

void ChangeDeleteItemWindow::rejectDialog()
{
    if(item != nullptr) {
        delete item;
        item = nullptr;
    }
    reject();
}
