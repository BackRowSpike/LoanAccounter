#ifndef ADDNEWCHANGEDELETEITEMWINDOW_H
#define ADDNEWCHANGEDELETEITEMWINDOW_H

#include <QDialog>
#include <QtWidgets>
#include "creditcontract.h"

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
QT_END_NAMESPACE

class AddNewChangeDeleteItemWindow : public QDialog
{
    Q_OBJECT

public:
    AddNewChangeDeleteItemWindow(QWidget *parent = nullptr);
protected:
    QLineEdit *borrowerFullName;
    QLineEdit *borrowerPassportNumber;
    QComboBox *borrowerType;
    QSpinBox *borrowerCreditRating;
    QLineEdit *firstPaymentDate;
    QLineEdit *contractNumber;
    QLineEdit *accountNumber;
    QLineEdit *loanValue;
    QComboBox *currency;
    QDoubleSpinBox *percent;
    QComboBox *repaymentMode;
    QSpinBox *paymentsCount;
    QComboBox *loanPurpose;
    QDoubleSpinBox *penaltyRate;
    QDoubleSpinBox *pledgeRate;
    QDialogButtonBox* buttonBox;
    QVBoxLayout *mainLayout;
    bool atributesValidation();
};

#endif // ADDNEWCHANGEDELETEITEMWINDOW_H
