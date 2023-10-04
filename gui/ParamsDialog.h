#ifndef PARAMSDIALOG_H
#define PARAMSDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class ParamsDialog; }
QT_END_NAMESPACE

class ParamsDialog : public QDialog
{
    Q_OBJECT

public:
    ParamsDialog(QWidget *parent = nullptr);
    ~ParamsDialog();

private:
    Ui::ParamsDialog *ui;

};
#endif // PARAMSDIALOG_H
