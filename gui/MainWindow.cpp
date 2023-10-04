#include "MainWindow.h"
#include "ParamsDialog.h"
#include "TreeNode.h"
#include "TreeSearchParallel.h"
#include "ui_MainWindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _listModel = new QStringListModel();
    ui->listView->setModel(_listModel);

    _paramsDialog = new ParamsDialog();

    _searchAlgo = std::make_unique<TreeSearchParallel>(4, std::make_shared<TreeNodeVectorCompact>(), false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_browseButton_clicked()
{
    QString selectedFile = QFileDialog::getOpenFileName(
        nullptr,          // parent widget
        "Load word list",      // dialog title
        "",               // starting directory (empty string means the default dir)
        "Text files (*.txt)"  // filter
        );


    // Set the file in the line edit
    if (!selectedFile.isEmpty())
    {
        ui->fileLineEdit->setText(selectedFile);
    }

}


void MainWindow::on_loadButton_clicked()
{
    if (_searchAlgo->loadWordList(ui->fileLineEdit->text().toStdString()) ) {
        ui->statusbar->showMessage("Loading successful !", 3000);
        ui->statusbar->setStyleSheet("color: rgb(0, 125, 0);");
    }
    else
    {
        ui->statusbar->showMessage("Loading failed !", 3000);
        ui->statusbar->setStyleSheet("color: rgb(125, 0, 0);");
    }
}


void MainWindow::on_paramButton_clicked()
{
    _paramsDialog->exec();
}


void MainWindow::on_searchButton_clicked()
{
    QString subStr = ui->searchLineEdit->text();
    if( !subStr.isEmpty() ) {
        unsigned long elapsed;
        std::vector<std::string> words = _searchAlgo->searchWordTimed(subStr.toStdString(), elapsed);
        ui->statusbar->showMessage( QString::number(words.size()) + " words found in " + QString::number(elapsed)  + "us", 3000);
        ui->statusbar->setStyleSheet("color: black");

        QStringList list;
        std::transform(words.begin(), words.end(), std::back_inserter(list), [](const std::string &v){ return QString::fromStdString(v); });
        _listModel->setStringList(list);


    }
}

