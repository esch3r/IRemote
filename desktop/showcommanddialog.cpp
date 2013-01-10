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

    for (int i = 0; i < command.length; i++)
    {
        time += command.data[i];
        x.append((double)time);
        y.append(!(i%2));
        x.append((double)time);
        y.append(i%2);
    }

    QwtPlotCurve *plotCurve = new QwtPlotCurve("Test");
    plotCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    plotCurve->setPen(QPen(qRgb(0,255,0)));
    //plotCurve->setAxes(xAxis, yAxis);

#if QWT_VERSION >= 0x060000
    plotCurve->setSamples(x,y);
#else
    plotCurve->setData(x,y);
#endif
    plotCurve->attach(ui->qwtPlot);
}
