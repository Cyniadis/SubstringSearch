#include "ParamsDialog.h"
#include "ui_ParamsDialog.h"

#include <QFileDialog>

ParamsDialog::ParamsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ParamsDialog)
{
    ui->setupUi(this);
}

ParamsDialog::~ParamsDialog()
{
    delete ui;
}
