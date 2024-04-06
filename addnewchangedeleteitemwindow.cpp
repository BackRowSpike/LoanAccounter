#include "addnewchangedeleteitemwindow.h"

AddNewChangeDeleteItemWindow::AddNewChangeDeleteItemWindow(QWidget *parent): QDialog(parent)
{
    //creating atribute fields
    borrowerFullName = new QLineEdit();
    borrowerPassportNumber = new QLineEdit();
    borrowerPassportNumber->setInputMask("0000000A000AA0");
    borrowerType = new QComboBox();
    borrowerType->addItem(tr("Private entity"));
    borrowerType->addItem(tr("Entity"));
    borrowerCreditRating = new QSpinBox();
    borrowerCreditRating->setRange(0, 999);
    firstPaymentDate = new QLineEdit();
    firstPaymentDate->setInputMask("00-00-0000");
    contractNumber = new QLineEdit();
    contractNumber->setInputMask("0000000000");
    accountNumber  = new QLineEdit();
    accountNumber->setInputMask("AA00AAAA00000000000000000000");
    loanValue = new QLineEdit();
    currency = new QComboBox();
    currency->addItem(tr("BYN"));
    currency->addItem(tr("USD"));
    percent = new QDoubleSpinBox();
    percent->setDecimals(2);
    percent->setRange(GenericParametrs::getOfficialBankRate(), 100);
    percent->setSingleStep(0.1);
    percent->setSuffix(" %");
    repaymentMode = new QComboBox();
    repaymentMode->addItem(tr("AN"));
    repaymentMode->addItem(tr("DIFF"));
    paymentsCount = new QSpinBox();
    paymentsCount->setRange(1, 360);
    loanPurpose = new QComboBox();
    loanPurpose->addItem(tr("Buyer"));
    loanPurpose->addItem(tr("Real property"));
    loanPurpose->addItem(tr("Replenishing working capital"));
    loanPurpose->addItem(tr("Ohter"));
    penaltyRate = new QDoubleSpinBox();
    penaltyRate->setDecimals(2);
    penaltyRate->setRange(0, 100);
    penaltyRate->setSingleStep(0.05);
    penaltyRate->setSuffix(" %");
    pledgeRate = new QDoubleSpinBox();
    pledgeRate->setDecimals(2);
    pledgeRate->setRange(0, 100);
    pledgeRate->setSingleStep(0.1);
    pledgeRate->setSuffix(" %");
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    //creating field lables
    QFormLayout *fieldsLayout = new QFormLayout;
    fieldsLayout->addRow(new QLabel(tr("Borrower full name:")), borrowerFullName);
    fieldsLayout->addRow(new QLabel(tr("Borrower password number:")), borrowerPassportNumber);
    fieldsLayout->addRow(new QLabel(tr("Borrower type:")), borrowerType);
    fieldsLayout->addRow(new QLabel(tr("Borrower credit rating:")), borrowerCreditRating);
    fieldsLayout->addRow(new QLabel(tr("First payment date:")), firstPaymentDate);
    fieldsLayout->addRow(new QLabel(tr("Contract number:")), contractNumber);
    fieldsLayout->addRow(new QLabel(tr("Account number:")), accountNumber);
    fieldsLayout->addRow(new QLabel(tr("Loan value:")), loanValue);
    fieldsLayout->addRow(new QLabel(tr("Currency:")), currency);
    fieldsLayout->addRow(new QLabel(tr("Percent:")), percent);
    fieldsLayout->addRow(new QLabel(tr("Repayment mode:")), repaymentMode);
    fieldsLayout->addRow(new QLabel(tr("Payments count:")), paymentsCount);
    fieldsLayout->addRow(new QLabel(tr("Loan purpose:")), loanPurpose);
    fieldsLayout->addRow(new QLabel(tr("Penalty rate:")), penaltyRate);
    fieldsLayout->addRow(new QLabel(tr("Pledge rate:")), pledgeRate);
    QGroupBox* fieldsGroupBox = new QGroupBox();
    fieldsGroupBox->setLayout(fieldsLayout);
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(fieldsGroupBox);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    resize(540, 500);
    setWindowIcon(QIcon("../LoanAccounter/source/logo.png"));
}

bool AddNewChangeDeleteItemWindow::atributesValidation()
{
    bool result = true;
    if(!borrowerFullName->text().isEmpty() && !CreditContract::validBorrowerFullName(borrowerFullName->text())) {
        borrowerFullName->setStyleSheet("color: red");
        result = false;
    } else {
        borrowerFullName->setStyleSheet("color: black");
    }
    if(!borrowerPassportNumber->text().isEmpty() && !CreditContract::validBorrowerPassportNumber(borrowerPassportNumber->text())) {
        borrowerPassportNumber->setStyleSheet("color: red");
        result = false;
    } else {
        borrowerPassportNumber->setStyleSheet("color: black");
    }
    if(!firstPaymentDate->text().isEmpty() && !firstPaymentDate->isReadOnly() && !CreditContract::validFirstPaymentDate(QDate::fromString(firstPaymentDate->text(), "dd-MM-yyyy"))) {
        firstPaymentDate->setStyleSheet("color: red");
        result = false;
    } else {
        firstPaymentDate->setStyleSheet("color: black");
    }
    if(!contractNumber->text().isEmpty() && !CreditContract::validContractNumber(contractNumber->text())) {
        contractNumber->setStyleSheet("color: red");
        result = false;
    } else {
        contractNumber->setStyleSheet("color: black");
    }
    if(!accountNumber->text().isEmpty() && !CreditContract::validAccountNumber(accountNumber->text())) {
        accountNumber->setStyleSheet("color: red");
        result = false;
    } else {
        accountNumber->setStyleSheet("color: black");
    }
    if(!loanValue->text().isEmpty() && !CreditContract::validLoanValue(loanValue->text().toFloat())) {
        loanValue->setStyleSheet("color: red");
        result = false;
    } else {
        loanValue->setStyleSheet("color: black");
    }
    if(!CreditContract::validPercent(percent->value())) {
        percent->setStyleSheet("color: red");
        result = false;
    } else {
        percent->setStyleSheet("color: black");
    }
    if(!CreditContract::validPaymentsCount(paymentsCount->value())) {
        paymentsCount->setStyleSheet("color: red");
        result = false;
    } else {
        paymentsCount->setStyleSheet("color: black");
    }
    if(!CreditContract::validPenaltyRate(penaltyRate->value())) {
        penaltyRate->setStyleSheet("color: red");
        result = false;
    } else {
        penaltyRate->setStyleSheet("color: black");
    }
    if(!CreditContract::validPledgeRate(pledgeRate->value())) {
        pledgeRate->setStyleSheet("color: red");
        result = false;
    } else {
        pledgeRate->setStyleSheet("color: black");
    }
    return result;
}
