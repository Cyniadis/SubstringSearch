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
    struct Params
    {
        bool _incrementalSearch;
        std::string _searchMethod;
        unsigned _nbThreads;
        bool _sortResults;
    };

    ParamsDialog(QWidget *parent = nullptr);
    ~ParamsDialog();

    Params getParams();

signals:
    void paramsDialogAccepted(ParamsDialog::Params& params);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ParamsDialog *ui;

};
#endif // PARAMSDIALOG_H
