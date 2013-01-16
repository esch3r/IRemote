#ifndef SHOWCOMMANDDIALOG_H
#define SHOWCOMMANDDIALOG_H

#include <QDialog>
#include <qwt_plot_curve.h>
#include "qwt_plot.h"
#include "iremote.h"

namespace Ui {
class ShowCommandDialog;
}

class ShowCommandDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ShowCommandDialog(QWidget *parent = 0);

    ~ShowCommandDialog();

    void loadIrCommand(const IrCommand &command);
    
private slots:
    void on_tableButton_clicked();

private:
    Ui::ShowCommandDialog *ui;
};

#endif // SHOWCOMMANDDIALOG_H
