#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ParamsDialog.h"
#include "SearchInterface.h"

#include <QMainWindow>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_browseButton_clicked();

    void on_loadButton_clicked();

    void on_paramButton_clicked();

    void on_searchButton_clicked();

private:
    Ui::MainWindow *ui;

    ParamsDialog *_paramsDialog;
    QStringListModel *_listModel;

    std::unique_ptr<SearchInterface> _searchAlgo;
};
#endif // MAINWINDOW_H

