#ifndef CHANGEDELETEITEMWINDOW_H
#define CHANGEDELETEITEMWINDOW_H

#include "addnewchangedeleteitemwindow.h"

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

class ChangeDeleteItemWindow : public AddNewChangeDeleteItemWindow
{
    Q_OBJECT

private:
    QPushButton* deleteItem;
    CreditContract* item;
signals:
    void creditContractChanged(CreditContract*);
    void creditContractDeleted(QString);
public:
    ChangeDeleteItemWindow(QWidget *parent = nullptr);
    void execute(CreditContract*, QDate);
private slots:
    void acceptDialog();
    void rejectDialog();
    void deleteItemClicked();
};

#endif // CHANGEDELETEITEMWINDOW_H
