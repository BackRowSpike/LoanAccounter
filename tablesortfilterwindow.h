#ifndef TABLESORTFILTERWINDOW_H
#define TABLESORTFILTERWINDOW_H

#include <QStandardItemModel>
#include "addnewitemwindow.h"
#include "changedeleteitemwindow.h"
#include "accounter.h"

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QSortFilterProxyModel;
class QTreeView;
class QCheckBox;
class QListWidget;
QT_END_NAMESPACE

class TableSortFilterWindow : public QWidget
{
    Q_OBJECT

public:
    TableSortFilterWindow();
    ~TableSortFilterWindow();

private slots:
    void filterRegularExpressionChanged();
    void filterColumnChanged();
    void addNewItemButtonClicked();
    void transactionPayButtonClicked();
    void sourceViewDoubleClicked(const QModelIndex &index);
    void proxyViewDoubleClicked(const QModelIndex &index);
    void testModeStateChanged(int);
    void dateIncrementation();
    void loanUnderpayment(QString);
    void loanRepayment(QString);
    void accounterChanged();

private:
    QSortFilterProxyModel *proxyModel;
    QGroupBox *sourceGroupBox;
    QGroupBox *proxyGroupBox;
    QGroupBox *transactionGroupBox;
    QTreeView *sourceView;
    QTreeView *proxyView;
    QLabel *filterPatternLabel;
    QLabel *filterColumnLabel;
    QLineEdit *filterPatternLineEdit;
    QComboBox *filterSyntaxComboBox;
    QComboBox *filterColumnComboBox;
    QPushButton *addNewItemButton;
    QLabel *currentDate;
    QCheckBox* testModeCheckBox;
    QLabel *paymentContractNumberLabel;
    QLabel *paymentValueLabel;
    QComboBox *paymentContractNumbers;
    QLineEdit *paymentValue;
    QListWidget *paymentHistory;
    QPushButton *paymentButton;

    Accounter* accounter;
    AddNewItemWindow* addnewitemwindow;
    ChangeDeleteItemWindow* changedeleteitemwindow;
    QStandardItemModel* model;
    QString dateBeforeTestMode;
    void createCreditContractModel(int row, QAbstractItemModel *model, QString bfn,
        QString bpn, QString bt, QString bcr, QString fpd, QString cn, QString an,
        QString lv, QString c, QString p, QString rm, QString pc, QString lp,
        QString pnr, QString plr, QString lb);
    void createCreditContractTable();
    void setSourceModel(QAbstractItemModel *model);
};

#endif // TABLESORTFILTERWINDOW_H
