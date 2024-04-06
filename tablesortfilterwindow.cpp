#include "tablesortfilterwindow.h"

static inline QColor textColor(const QPalette &palette)
{
    return palette.color(QPalette::Active, QPalette::Text);
}

static void setTextColor(QWidget *w, const QColor &c)
{
    auto palette = w->palette();
    if (textColor(palette) != c) {
        palette.setColor(QPalette::Active, QPalette::Text, c);
        w->setPalette(palette);
    }
}

TableSortFilterWindow::TableSortFilterWindow()
{
    proxyModel = new QSortFilterProxyModel;

    sourceView = new QTreeView;
    sourceView->setRootIsDecorated(false);
    sourceView->setAlternatingRowColors(true);
    sourceView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    sourceView->setSelectionBehavior(QAbstractItemView::SelectRows);
    sourceView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(sourceView, &QAbstractItemView::doubleClicked,
                this, &TableSortFilterWindow::sourceViewDoubleClicked);

    proxyView = new QTreeView;
    proxyView->setRootIsDecorated(false);
    proxyView->setAlternatingRowColors(true);
    proxyView->setModel(proxyModel);
    proxyView->setSortingEnabled(true);
    proxyView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    proxyView->setSelectionBehavior(QAbstractItemView::SelectRows);
    proxyView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(proxyView, &QAbstractItemView::doubleClicked,
                this, &TableSortFilterWindow::proxyViewDoubleClicked);

    filterPatternLineEdit = new QLineEdit;
    filterPatternLineEdit->setClearButtonEnabled(true);
    filterPatternLabel = new QLabel(tr("&Filter pattern:"));
    filterPatternLabel->setBuddy(filterPatternLineEdit);
    filterColumnComboBox = new QComboBox;
    filterColumnComboBox->addItem(tr("Borrower name"));
    filterColumnComboBox->addItem(tr("Borrower passport number"));
    filterColumnComboBox->addItem(tr("Borrower type"));
    filterColumnComboBox->addItem(tr("Borrower credit rating"));
    filterColumnComboBox->addItem(tr("Contract date"));
    filterColumnComboBox->addItem(tr("Contract number"));
    filterColumnComboBox->addItem(tr("Account number"));
    filterColumnComboBox->addItem(tr("Loan value"));
    filterColumnComboBox->addItem(tr("Currency"));
    filterColumnComboBox->addItem(tr("Percent"));
    filterColumnComboBox->addItem(tr("Repayment mode"));
    filterColumnComboBox->addItem(tr("Periods"));
    filterColumnComboBox->addItem(tr("Interval"));
    filterColumnComboBox->addItem(tr("Purpose"));
    filterColumnComboBox->addItem(tr("PenaltyRate"));
    filterColumnComboBox->addItem(tr("PledgeRate"));
    filterColumnLabel = new QLabel(tr("Filter &column:"));
    filterColumnLabel->setBuddy(filterColumnComboBox);

    connect(filterPatternLineEdit, &QLineEdit::textChanged,
            this, &TableSortFilterWindow::filterRegularExpressionChanged);
    connect(filterColumnComboBox, &QComboBox::currentIndexChanged,
            this, &TableSortFilterWindow::filterColumnChanged);

    sourceGroupBox = new QGroupBox(tr("Original table"));
    proxyGroupBox = new QGroupBox(tr("Sorted/Filtered table"));
    transactionGroupBox = new QGroupBox(tr("Payments"));

    QHBoxLayout *sourceLayout = new QHBoxLayout;
    sourceLayout->addWidget(sourceView);
    sourceGroupBox->setLayout(sourceLayout);

    addNewItemButton = new QPushButton();
    addNewItemButton->setText("Add new credit contract");
    connect(addNewItemButton, &QPushButton::clicked,
            this, &TableSortFilterWindow::addNewItemButtonClicked);

    testModeCheckBox = new QCheckBox("Test mode", this);
    testModeCheckBox->setCheckState(Qt::Unchecked);
    connect(testModeCheckBox, &QCheckBox::stateChanged,
            this, &TableSortFilterWindow::testModeStateChanged);

    currentDate = new QLabel();
    currentDate->setText(QDateTime::currentDateTime().toString("dd-MM-yyyy"));
    currentDate->setAlignment(Qt::AlignRight);

    QGridLayout *proxyLayout = new QGridLayout;
    proxyLayout->addWidget(proxyView, 0, 0, 1, 16);
    proxyLayout->addWidget(filterPatternLabel, 1, 0);
    proxyLayout->addWidget(filterPatternLineEdit, 1, 1, 1, 2);
    proxyLayout->addWidget(testModeCheckBox, 1, 3);
    proxyLayout->addWidget(filterColumnLabel, 2, 0);
    proxyLayout->addWidget(filterColumnComboBox, 2, 1, 1, 2);
    proxyLayout->addWidget(addNewItemButton, 2, 3);
    proxyLayout->addWidget(currentDate, 2, proxyLayout->columnCount()-1);
    proxyGroupBox->setLayout(proxyLayout);

    paymentContractNumbers = new QComboBox();
    paymentValue = new QLineEdit();
    paymentHistory = new QListWidget();
    paymentButton = new QPushButton();
    paymentButton->setText("Pay");
    connect(paymentButton, &QPushButton::clicked,
            this, &TableSortFilterWindow::transactionPayButtonClicked);

    QGridLayout *transactionLayout = new QGridLayout;
    paymentContractNumberLabel = new QLabel(tr("Contract number:"));
    paymentContractNumberLabel->adjustSize();
    paymentContractNumberLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    paymentValueLabel = new QLabel(tr("Payment value:"));
    paymentValueLabel->adjustSize();
    paymentValueLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    transactionLayout->addWidget(paymentContractNumberLabel, 0, 0);
    transactionLayout->addWidget(paymentContractNumbers, 0, 1);
    transactionLayout->addWidget(paymentValueLabel, 1, 0);
    transactionLayout->addWidget(paymentValue, 1, 1);
    transactionLayout->addWidget(paymentButton, 2, 0, 1, 2);
    transactionLayout->addWidget(paymentHistory, 3, 0, 1, 2);
    transactionGroupBox->setLayout(transactionLayout);

    QGridLayout *mainLayout = new QGridLayout;
    transactionGroupBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    mainLayout->addWidget(sourceGroupBox, 0, 0, 1, 16);
    mainLayout->addWidget(proxyGroupBox, 1, 0, 1, 16);
    mainLayout->addWidget(transactionGroupBox, 0, 16, 2, 2);
    setLayout(mainLayout);
    resize(1200, 600);
    setWindowTitle(tr("Loan Accounter"));
    setWindowIcon(QIcon("../LoanAccounter/source/logo.png"));

    proxyView->sortByColumn(1, Qt::AscendingOrder);
    filterColumnComboBox->setCurrentIndex(1);
    filterPatternLineEdit->setText("");

    model = new QStandardItemModel(0, 16, this);

    try {
        accounter = new Accounter();
        connect(accounter, &Accounter::accounterChanged, this,
                &TableSortFilterWindow::accounterChanged);
        connect(accounter, &Accounter::dateIncrementation,
                this, &TableSortFilterWindow::dateIncrementation);
        connect(accounter, &Accounter::loanUnderpayment,
                this, &TableSortFilterWindow::loanUnderpayment);
        connect(accounter, &Accounter::loanRepayment,
                this, &TableSortFilterWindow::loanRepayment);

        addnewitemwindow = new AddNewItemWindow(this);
        connect(addnewitemwindow, &AddNewItemWindow::addNewCreditContract,
                accounter, &Accounter::addNewCreditContract);

        changedeleteitemwindow = new ChangeDeleteItemWindow(this);
        connect(changedeleteitemwindow, &ChangeDeleteItemWindow::creditContractChanged,
                accounter, &Accounter::changeCreditContract);
        connect(changedeleteitemwindow, &ChangeDeleteItemWindow::creditContractDeleted,
                accounter, &Accounter::deleteCreditContract);

        createCreditContractTable();
    } catch(const ProcessingException &e) {
        QString message = "{" + e.getMessage() + "} in TableSortFilterWindow::TableSortFilterWindow()";
        QMessageBox::critical(nullptr, tr("Loan Accounter"),
                    tr(message.toStdString().c_str()), QMessageBox::Cancel);
    }
}

TableSortFilterWindow::~TableSortFilterWindow()
{
    delete addnewitemwindow;
    delete changedeleteitemwindow;
    delete accounter;
}

void TableSortFilterWindow::setSourceModel(QAbstractItemModel *model)
{
    proxyModel->setSourceModel(model);
    sourceView->setModel(model);
    for(int i = 0; i < 16; i++) {
        proxyView->resizeColumnToContents(i);
        sourceView->resizeColumnToContents(i);
    }
}

void TableSortFilterWindow::filterRegularExpressionChanged()
{
    QString pattern = filterPatternLineEdit->text();
    QRegularExpression regularExpression(pattern, QRegularExpression::NoPatternOption);
    if (regularExpression.isValid()) {
        filterPatternLineEdit->setToolTip(QString());
        proxyModel->setFilterRegularExpression(regularExpression);
        setTextColor(filterPatternLineEdit, textColor(style()->standardPalette()));
    } else {
        filterPatternLineEdit->setToolTip(regularExpression.errorString());
        proxyModel->setFilterRegularExpression(QRegularExpression());
        setTextColor(filterPatternLineEdit, Qt::red);
    }
}

void TableSortFilterWindow::filterColumnChanged()
{
    proxyModel->setFilterKeyColumn(filterColumnComboBox->currentIndex());
}

void TableSortFilterWindow::addNewItemButtonClicked()
{
    try {
        addnewitemwindow->execute(QDate::fromString(currentDate->text(), "dd-MM-yyyy"));
    } catch(const ProcessingException &e) {
        QString message = "{" + e.getMessage() + "} in TableSortFilterWindow::addNewItemButtonClicked()";
        QMessageBox::critical(nullptr, tr("Loan Accounter"),
                    tr(message.toStdString().c_str()), QMessageBox::Cancel);
    }
}

void TableSortFilterWindow::transactionPayButtonClicked()
{
    if(paymentContractNumbers->currentIndex() == -1 || paymentContractNumberLabel->text().isEmpty()) return;
    bool ok = false;
    float value = paymentValue->text().toFloat(&ok);
    if(!ok) {
        paymentValue->setStyleSheet("color: red");
    } else try {
        if(value < 0) {
            paymentValue->setStyleSheet("color: red");
            return;
        }
        QString cn = paymentContractNumbers->currentText();
        accounter->setNewTransaction(cn, -1*value);
        paymentValue->clear();
        paymentValue->setStyleSheet("color: black");
        paymentHistory->addItem(cn + "   :   " + QString::number(value, 'f', 2) + "    " + GenericParametrs::currencyEnumToString(accounter->findInCatalogue(cn)->getCurrency()));
    } catch(const ProcessingException &e) {
        QString message = "{" + e.getMessage() + "} in TableSortFilterWindow::transactionPayButtonClicked()";
        QMessageBox::critical(nullptr, tr("Loan Accounter"),
                    tr(message.toStdString().c_str()), QMessageBox::Cancel);
    }
}

void TableSortFilterWindow::sourceViewDoubleClicked(const QModelIndex &index)
{
    QAbstractItemModel* model = sourceView->model();
    QModelIndex cn_index = model->index(index.row(), 1);
    QVariant cn = model->data(cn_index, 0);
    try {
        changedeleteitemwindow->execute(accounter->findInCatalogue(cn.toString()), QDate::fromString(currentDate->text(), "dd-MM-yyyy"));
    } catch(const ProcessingException &e) {
        QString message = "{" + e.getMessage() + "} in TableSortFilterWindow::sourceViewDoubleClicked()";
        QMessageBox::critical(nullptr, tr("Loan Accounter"),
                    tr(message.toStdString().c_str()), QMessageBox::Cancel);
    }
}

void TableSortFilterWindow::proxyViewDoubleClicked(const QModelIndex &index)
{
    QAbstractItemModel* model = proxyView->model();
    QModelIndex cn_index = model->index(index.row(), 1);
    QVariant cn = model->data(cn_index, 0);
    try {
        changedeleteitemwindow->execute(accounter->findInCatalogue(cn.toString()), QDate::fromString(currentDate->text(), "dd-MM-yyyy"));
    } catch(const ProcessingException &e) {
        QString message = "{" + e.getMessage() + "} in TableSortFilterWindow::proxyViewDoubleClicked()";
        QMessageBox::critical(nullptr, tr("Loan Accounter"),
                    tr(message.toStdString().c_str()), QMessageBox::Cancel);
    }
}

void TableSortFilterWindow::testModeStateChanged(int state)
{
    if(state == 0) {
        currentDate->setText(dateBeforeTestMode);
    } else {
        dateBeforeTestMode = currentDate->text();
    }
    accounter->testModeStateChanged();
    paymentHistory->clear();
    paymentValue->clear();
}

void TableSortFilterWindow::dateIncrementation()
{
    currentDate->setText(QDate::fromString(currentDate->text(), "dd-MM-yyyy").addDays(1).toString("dd-MM-yyyy"));
}

void TableSortFilterWindow::loanUnderpayment(QString cn)
{
    QString message = "Credit contract '" + cn + "' has reached second underpayment in a row.";
    QMessageBox::information(this, tr("Loan Accounter"),
            tr(message.toStdString().c_str()),
            QMessageBox::Ok);
}

void TableSortFilterWindow::loanRepayment(QString cn)
{
    QString message = "Credit contract '" + cn + "' has been repaid.";
    QMessageBox::information(this, tr("Loan Accounter"),
            tr(message.toStdString().c_str()),
            QMessageBox::Ok);
}

void TableSortFilterWindow::accounterChanged()
{
    try {
        createCreditContractTable();
    } catch(const ProcessingException &e) {
        QString message = "{" + e.getMessage() + "} in TableSortFilterWindow::accounterChanged()";
        QMessageBox::critical(nullptr, tr("Loan Accounter"),
                    tr(message.toStdString().c_str()), QMessageBox::Cancel);
    }
}

void TableSortFilterWindow::createCreditContractModel(int row, QAbstractItemModel *model, QString bfn,
    QString bpn, QString bt, QString bcr, QString fpd, QString cn, QString an, QString lv,
    QString c, QString p, QString rm, QString pc, QString lp, QString pnr, QString plr, QString lb)
{
    model->insertRow(row);
    model->setData(model->index(row, 0), bfn);
    model->setData(model->index(row, 8), bpn);
    model->setData(model->index(row, 9), bt);
    model->setData(model->index(row, 10), bcr);
    model->setData(model->index(row, 7), fpd);
    model->setData(model->index(row, 1), cn);
    model->setData(model->index(row, 11), an);
    model->setData(model->index(row, 2), lv);
    model->setData(model->index(row, 5), c);
    model->setData(model->index(row, 6), p);
    model->setData(model->index(row, 12), rm);
    model->setData(model->index(row, 4), pc);
    model->setData(model->index(row, 13), lp);
    model->setData(model->index(row, 14), pnr);
    model->setData(model->index(row, 15), plr);
    model->setData(model->index(row, 3), lb);
}

void TableSortFilterWindow::createCreditContractTable()
{
    model->clear();
    model->setRowCount(0);
    model->setColumnCount(16);
    model->setParent(this);
    /*model->setHeaderData(0, Qt::Horizontal, QObject::tr("Borrower full name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Borrower passport number"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Borrower type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Borrower credit rating"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("First payment date"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Contract number"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Account number"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Loan value"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Currency"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Percent"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Repayment mode"));
    model->setHeaderData(11, Qt::Horizontal, QObject::tr("Payments count"));
    model->setHeaderData(12, Qt::Horizontal, QObject::tr("Loan purpose"));
    model->setHeaderData(13, Qt::Horizontal, QObject::tr("Penalty rate"));
    model->setHeaderData(14, Qt::Horizontal, QObject::tr("Pledge rate"));
    model->setHeaderData(15, Qt::Horizontal, QObject::tr("Loan balance"));*/
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Borrower full name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Contract number"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Loan value"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Loan balance"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Payments count"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Currency"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Percent"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("First payment date"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Borrower passport number"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Borrower type"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Borrower credit rating"));
    model->setHeaderData(11, Qt::Horizontal, QObject::tr("Account number"));
    model->setHeaderData(12, Qt::Horizontal, QObject::tr("Repayment mode"));
    model->setHeaderData(13, Qt::Horizontal, QObject::tr("Loan purpose"));
    model->setHeaderData(14, Qt::Horizontal, QObject::tr("Penalty rate"));
    model->setHeaderData(15, Qt::Horizontal, QObject::tr("Pledge rate"));
    QString currentTransactionContractNumber = paymentContractNumbers->currentText();
    paymentContractNumbers->clear();
    try {
        CreditContract* cc;
        for (int i = 0; i < accounter->size(); i++) {
            cc = accounter->at(i);
            createCreditContractModel(i, model,
                                      cc->getBorrowerFullName(),
                                      cc->getBorrowerPassportNumber(),
                                      GenericParametrs::borrowerTypeEnumToString(cc->getBorrowerType()),
                                      QString::number(cc->getBorrowerCreditRating()),
                                      cc->getFirstPaymentDate().toString("dd-MM-yyyy"),
                                      cc->getContractNumber(),
                                      cc->getAccountNumber(),
                                      QString::number(cc->getLoanValue(), 'f', 2),
                                      GenericParametrs::currencyEnumToString(cc->getCurrency()),
                                      QString::number(cc->getPercent()),
                                      GenericParametrs::repaymentModeEnumToString(cc->getRepaymentMode()),
                                      QString::number(cc->getPaymentsCount()),
                                      GenericParametrs::loanPurposeEnumToString(cc->getLoanPurpose()),
                                      QString::number(cc->getPenaltyRate()),
                                      QString::number(cc->getPledgeRate()),
                                      QString::number(accounter->getAccountBalance(cc->getAccountNumber())));
            if(accounter->hasUnderpayment(cc->getContractNumber())) {
                for (int j = 0; j < model->columnCount(); j++) {
                    model->item(i, j)->setBackground(QColor(255,0,0,76));
                }
            }
            paymentContractNumbers->addItem(cc->getContractNumber());
            if (!currentTransactionContractNumber.isEmpty() && cc->getContractNumber() == currentTransactionContractNumber) {
                paymentContractNumbers->setCurrentIndex(i);
                currentTransactionContractNumber = "";
            }
        }
        setSourceModel(model);
        if(!currentTransactionContractNumber.isEmpty()) {
            paymentContractNumbers->setCurrentIndex(-1);
        }
    } catch(const ProcessingException &e) {
        throw ProcessingException("{" + e.getMessage() + "} in TableSortFilterWindow::createCreditContractTable()");
    }
}
