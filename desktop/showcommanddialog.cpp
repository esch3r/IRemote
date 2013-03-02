#include "showcommanddialog.h"
#include "ui_showcommanddialog.h"

ShowCommandDialog::ShowCommandDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowCommandDialog)
{
    ui->setupUi(this);

    plotCurve = NULL;

    ui->qwtPlot->setAutoFillBackground( true );
    QPalette p = ui->qwtPlot->palette();
    p.setColor(QPalette::Window, Qt::black);
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::Foreground, Qt::white);
    ui->qwtPlot->setPalette(p);

    ui->tableWidget->setVisible(false);
    ui->listAddButton->setVisible(false);
    ui->listRemoveButton->setVisible(false);
    ui->listRefreshButton->setVisible(false);
}

ShowCommandDialog::~ShowCommandDialog()
{
    delete ui;
}

void ShowCommandDialog::on_tableButton_clicked()
{
    if (ui->tableWidget->isVisible())
    {
        ui->tableWidget->setVisible(false);
        ui->listAddButton->setVisible(false);
        ui->listRemoveButton->setVisible(false);
        ui->listRefreshButton->setVisible(false);
        ui->tableButton->setIcon(QIcon::fromTheme("arrow-left"));
    }
    else
    {
        ui->tableWidget->setVisible(true);
        ui->listAddButton->setVisible(true);
        ui->listRemoveButton->setVisible(true);
        ui->listRefreshButton->setVisible(true);
        ui->tableButton->setIcon(QIcon::fromTheme("arrow-right"));
    }
}

void ShowCommandDialog::on_listAddButton_clicked()
{
    int row = ui->tableWidget->currentRow();

    if (row != -1)
    {
         ui->tableWidget->insertRow(row);
         QTableWidgetItem *item = new QTableWidgetItem();
         item->setText(QString::fromUtf8("%1").arg(0));
         item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
         ui->tableWidget->setItem(row,0,item);
    }
}

void ShowCommandDialog::on_listRemoveButton_clicked()
{
    int row = ui->tableWidget->currentRow();

    if (row != -1)
    {
        ui->tableWidget->removeRow(row);
    }
}

void ShowCommandDialog::on_listRefreshButton_clicked()
{
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        m_remoteCommand.data[i] = ui->tableWidget->item(i, 0)->text().toInt();
    }

    m_remoteCommand.length = ui->tableWidget->rowCount();
    refreshCommand();
    emit remoteCommandChanged(m_remoteCommand);
}

void ShowCommandDialog::refreshCommand()
{
    QVector<double> x;
    QVector<double> y;
    double time = 0;

    ui->tableWidget->clear();

    if (plotCurve != NULL) {
        plotCurve->detach();
        delete plotCurve;
    }

    x.append(0);
    y.append(0);
    x.append(0);
    y.append(1);

    ui->tableWidget->setRowCount(m_remoteCommand.length);

    for (int i = 0; i < m_remoteCommand.length; i++)
    {
        time += m_remoteCommand.data[i];
        x.append((double)time);
        y.append(!(i%2));
        x.append((double)time);
        y.append(i%2);
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(QString::fromUtf8("%1").arg(m_remoteCommand.data[i]));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
        ui->tableWidget->setItem(i,0,item);
    }

    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,0,1,1);
    ui->qwtPlot->setAxisMaxMinor(QwtPlot::yLeft,1);

    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0,x.last());

    plotCurve = new QwtPlotCurve("Data");
    plotCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    plotCurve->setPen(QPen(qRgb(0,255,0)));

#if QWT_VERSION >= 0x060000
    plotCurve->setSamples(x,y);
#else
    plotCurve->setData(x,y);
#endif
    plotCurve->attach(ui->qwtPlot);
    ui->qwtPlot->replot();

    if (m_remoteCommand.medium == 0) {
        this->setWindowTitle("IR Command");
    } else if (m_remoteCommand.medium == 1) {
        this->setWindowTitle("433MHz Command");
    } else if (m_remoteCommand.medium == 2) {
        this->setWindowTitle("868MHz Command");
    }
}
