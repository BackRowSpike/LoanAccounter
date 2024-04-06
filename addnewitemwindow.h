#ifndef ADDNEWITEMWINDOW_H
#define ADDNEWITEMWINDOW_H

#include "addnewchangedeleteitemwindow.h"

class AddNewItemWindow : public AddNewChangeDeleteItemWindow
{
    Q_OBJECT

public:
    AddNewItemWindow(QWidget *parent = nullptr);
    void execute(QDate);
signals:
    void addNewCreditContract(CreditContract*);
private slots:
    void acceptDialog();
};

#endif // ADDNEWITEMWINDOW_H
