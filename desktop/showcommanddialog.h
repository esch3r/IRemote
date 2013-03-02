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
    Q_PROPERTY(RemoteCommand remoteCommand READ remoteCommand WRITE setRemoteCommand NOTIFY remoteCommandChanged)
    
public:
    explicit ShowCommandDialog(QWidget *parent = 0);

    ~ShowCommandDialog();

    RemoteCommand remoteCommand() const
    {
        return m_remoteCommand;
    }

public slots:
    void setRemoteCommand(const RemoteCommand &arg)
    {
        m_remoteCommand = arg;
        refreshCommand();
    }

signals:
    void remoteCommandChanged(RemoteCommand arg);

private slots:
    void on_tableButton_clicked();

    void on_listAddButton_clicked();

    void on_listRemoveButton_clicked();

    void on_listRefreshButton_clicked();

private:
    Ui::ShowCommandDialog *ui;

    RemoteCommand m_remoteCommand;
    QwtPlotCurve *plotCurve;

    void refreshCommand();
};

#endif // SHOWCOMMANDDIALOG_H
