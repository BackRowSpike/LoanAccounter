#include "addnewitemwindow.h"

AddNewItemWindow::AddNewItemWindow(QWidget *parent): AddNewChangeDeleteItemWindow::AddNewChangeDeleteItemWindow(parent)
{
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AddNewItemWindow::acceptDialog);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    setWindowTitle(tr("Add new credit contract"));
    resize(540, 470);
}

void AddNewItemWindow::execute(QDate curDate)
{
    borrowerFullName->clear();
    borrowerPassportNumber->clear();
    borrowerType->setCurrentIndex(0);
    borrowerCreditRating->setValue(450);
    firstPaymentDate->setText(curDate.addDays(1).toString("dd-MM-yyyy"));
    contractNumber->clear();
    accountNumber->clear();
    loanValue->clear();
    currency->setCurrentIndex(0);
    percent->setValue(GenericParametrs::getOfficialBankRate());
    repaymentMode->setCurrentIndex(0);
    paymentsCount->setValue(12);
    loanPurpose->setCurrentIndex(0);
    penaltyRate->setValue(0);
    pledgeRate->setValue(0);
    this->exec();
}

void AddNewItemWindow::acceptDialog()
{
    if (atributesValidation()) {
        BorrowerTypeEnum bt;
        switch(borrowerType->currentIndex()) {
            case 1:
                bt = BorrowerTypeEnum::Entity;
                break;
            default:
                bt = BorrowerTypeEnum::PrivateEntity;
                break;
        }
        CurrencyEnum c;
        switch(currency->currentIndex()) {
        case 1:
            c = CurrencyEnum::USD;
            break;
        default:
            c = CurrencyEnum::BYN;
            break;
        }
        RepaymentModeEnum rm;
        switch(repaymentMode->currentIndex()) {
        case 1:
            rm = RepaymentModeEnum::DIFF;
            break;
        default:
            rm = RepaymentModeEnum::AN;
            break;
        }
        LoanPurposeEnum lp;
        switch(loanPurpose->currentIndex()) {
        case 0:
            lp = LoanPurposeEnum::Buyer;
            break;
        case 1:
            lp = LoanPurposeEnum::RealProperty;
            break;
        case 2:
            lp = LoanPurposeEnum::ReplenishingWorkingCapital;
            break;
        default:
            lp = LoanPurposeEnum::Ohter;
            break;
        }
        try {
            CreditContract* cc = new CreditContract(borrowerFullName->text(),
                        borrowerPassportNumber->text(), bt, borrowerCreditRating->value(),
                        QDate::fromString(firstPaymentDate->text(), "dd-MM-yyyy"), contractNumber->text(),
                        accountNumber->text(), loanValue->text().toFloat(), c, percent->value(),
                        rm, paymentsCount->value(), lp, pledgeRate->value(), penaltyRate->value());
            emit addNewCreditContract(cc);
        } catch(const ProcessingException &e) {
            throw ProcessingException("{" + e.getMessage() + "} in AddNewItemWindow::acceptDialog()");
        }
        accept();
    }
}
