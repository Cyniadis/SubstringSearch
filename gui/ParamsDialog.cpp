#include "ParamsDialog.h"
#include "ui_ParamsDialog.h"

#include <QFileDialog>
#include <thread>

ParamsDialog::ParamsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ParamsDialog)
{
    ui->setupUi(this);
    ui->nbThreads_spinBox->setMaximum(std::thread::hardware_concurrency());
}

ParamsDialog::~ParamsDialog()
{
    delete ui;
}

void ParamsDialog::on_buttonBox_accepted()
{
    Params params = getParams();
    emit paramsDialogAccepted(params);
}

ParamsDialog::Params ParamsDialog::getParams()
{
    Params params;
    params._incrementalSearch = ui->incrementalSearch_checkBox->isChecked();
    params._nbThreads = ui->nbThreads_spinBox->value();
    params._searchMethod = ui->searchMethod_comboBox->currentText().toStdString();
    params._sortResults = ui->sort_checkBox->isChecked();

    return params;
}

