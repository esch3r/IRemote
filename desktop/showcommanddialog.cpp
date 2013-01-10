#include "showcommanddialog.h"
#include "ui_showcommanddialog.h"

ShowCommandDialog::ShowCommandDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowCommandDialog)
{
    ui->setupUi(this);

    ui->qwtPlot->setAutoFillBackground( true );
    QPalette p = ui->qwtPlot->palette();
    p.setColor(QPalette::Window, Qt::black);
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::Foreground, Qt::white);
    ui->qwtPlot->setPalette(p);

    ui->tableWidget->setVisible(false);
}

ShowCommandDialog::~ShowCommandDialog()
{
    delete ui;
}

void ShowCommandDialog::loadIrCommand(const IrCommand &command)
{
    QVector<double> x;
    QVector<double> y;
    double time = 0;

    x.append(0);
    y.append(0);
    x.append(0);
    y.append(1);

    ui->tableWidget->setRowCount(command.length-1);

    for (int i = 0; i < command.length; i++)
    {
        time += command.data[i];
        x.append((double)time);
        y.append(!(i%2));
        x.append((double)time);
        y.append(i%2);
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(QString::fromUtf8("%1 µs").arg(command.data[i]));
        item->setFlags(Qt::ItemIsEnabled);
        ui->tableWidget->setItem(i,0,item);
    }

    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,0,1,1);
    ui->qwtPlot->setAxisMaxMinor(QwtPlot::yLeft,1);

    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0,x.last());

    QwtPlotCurve *plotCurve = new QwtPlotCurve("Data");
    plotCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    plotCurve->setPen(QPen(qRgb(0,255,0)));

#if QWT_VERSION >= 0x060000
    plotCurve->setSamples(x,y);
#else
    plotCurve->setData(x,y);
#endif
    plotCurve->attach(ui->qwtPlot);
}

void ShowCommandDialog::on_tableButton_clicked()
{
    if (ui->tableWidget->isVisible())
    {
        ui->tableWidget->setVisible(false);
        ui->tableButton->setIcon(QIcon::fromTheme("arrow-left"));
    }
    else
    {
        ui->tableWidget->setVisible(true);
        ui->tableButton->setIcon(QIcon::fromTheme("arrow-right"));
    }
}
