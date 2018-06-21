#include "window.h"
#include "ui_window.h"
#include "gridlabel.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QFile>
#include <QTextStream>

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window),
    editable(false),
    curGrid(nullptr)
{
    ui->setupUi(this);
    printf("Window w=%d,h=%d\n",width(), height());
    //record all grids in container
    initGridsContainer();
    //connect
    registerGrids();

    data = new SudokuData;
    connect(data, SIGNAL(potentialInitialized(int,QSet<char>)),this,SLOT(on_potentialInitialized(int,QSet<char>)));
    connect(data,SIGNAL(remove_digit_from_potentials(int,char)),this,SLOT(on_remove_digit_from_potentials(int,char)));
    connect(data, SIGNAL(update_grid_number(int,char)), this, SLOT(on_update_grid_number(int,char)));
}

Window::~Window()
{
    delete ui;
}

void Window::clearBoard()
{
    //ui
    foreach (GridLabel* grid, grids) {
        grid->setText("");
    }
}

QVector<char> Window::getDatemFromBoard()
{
    QVector<char> datem;
    foreach (GridLabel* grid, grids) {
        char c = grid->getNumber();
        qDebug("getDatem grid number:[%c]",c);
        datem.append(c);
    }
    return datem;
}

QVector<char> Window::getDatemFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("sudoku file"),QStringLiteral("."),QStringLiteral("Text files (*.txt)"));
    QFile srcFile(fileName);
    if (!srcFile.open(QIODevice::ReadOnly|QIODevice::Text)) {
        return QVector<char>();
    }

    QVector<char> datem;
    QTextStream stream(&srcFile);
    QString line;
    int n = 0;
    while (stream.readLineInto(&line)) {
        if (line.size() < 9)
            continue;
        do {
            if (line[n%9].toLatin1() != '0')
                datem.push_back(line[n%9].toLatin1());
            else
                datem.push_back(' ');
            ++n;
        } while (n%9 != 0);
    }
    return datem;
}

void Window::setDatemToBoard(const QVector<char> &datem)
{
    int index = 0;
    foreach (GridLabel* grid, grids) {
        grid->setNumber(datem[index]);
        ++index;
    }
}

void Window::keyPressEvent(QKeyEvent *ev)
{
    qDebug("ket pressed:%c",ev->key());
    if (editable && curGrid != nullptr) {
        int key = ev->key();
        switch (key) {
        case Qt::Key_0:
            curGrid->clearNumber();
            break;
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
            curGrid->setNumber(key);
            break;
        default:
            break;
        }
    }
}

void Window::on_potentialInitialized(int index, QSet<char> potentials)
{
    qDebug("index:%d",index);
    GridLabel *grid = grids[index];
    grid->setPotentialNumbers(potentials);
}

void Window::on_remove_digit_from_potentials(int index, char num)
{
    GridLabel *grid = grids[index];
    grid->remove_digit_from_potentials(num);
    ui->textBrowser->append(QString("<font size=\"2\" color=\"green\">remove potential [%1] from digit[%2]</font>").arg(num).arg(index));
}

void Window::on_update_grid_number(int index, char num)
{
    GridLabel *grid = grids[index];
    grid->setNumber(num);
    ui->textBrowser->append(QString("<font size=\"3\" color=\"blue\">verify [%1] in digit[%2]</font>").arg(num).arg(index));
}

void Window::on_focused_grid()
{
    QObject *source = sender();
    GridLabel *grid = qobject_cast<GridLabel*>(source);
    if (grid != nullptr)
        curGrid = grid;
}

/*file content like:
203147009
810900301
...
...
*/
void Window::on_importBtn_clicked()
{
    QVector<char> datem = getDatemFromFile();
    if (datem.size() < 81)
        return;

    clearBoard();
    setDatemToBoard(datem);
    setGridsDiseditable();
    data->initializeDatem(datem);

    ui->SolveBtn->setDisabled(false);
}

void Window::on_DiyBtn_clicked(bool checked)
{
    if (checked) {
        clearBoard();
        setGridsEditable();

        ui->DiyBtn->setText("Complete");
    }
    else {
        QVector<char> datem = getDatemFromBoard();
        setGridsDiseditable();
        data->initializeDatem(datem);

        ui->DiyBtn->setText("DIY");
    }

    ui->SolveBtn->setDisabled(checked);
}

void Window::on_SolveBtn_clicked()
{
    if (data->checkComplete()) {
        ui->textBrowser->append(QString("<font size=\"4\" color=\"red\">Sudoku Solved!</font>"));
        return;
    }

    data->solve();
}

void Window::setGridsEditable()
{
    foreach (GridLabel *grid, grids) {
        grid->setEditable(true);
    }
    editable = true;
}

void Window::setGridsDiseditable()
{
    editable = false;
    foreach (GridLabel *grid, grids) {
        grid->setEditable(false);
    }
}

void Window::initGridsContainer()
{
    grids.append(ui->grid00);
    grids.append(ui->grid01);
    grids.append(ui->grid02);
    grids.append(ui->grid03);
    grids.append(ui->grid04);
    grids.append(ui->grid05);
    grids.append(ui->grid06);
    grids.append(ui->grid07);
    grids.append(ui->grid08);

    grids.append(ui->grid10);
    grids.append(ui->grid11);
    grids.append(ui->grid12);
    grids.append(ui->grid13);
    grids.append(ui->grid14);
    grids.append(ui->grid15);
    grids.append(ui->grid16);
    grids.append(ui->grid17);
    grids.append(ui->grid18);

    grids.append(ui->grid20);
    grids.append(ui->grid21);
    grids.append(ui->grid22);
    grids.append(ui->grid23);
    grids.append(ui->grid24);
    grids.append(ui->grid25);
    grids.append(ui->grid26);
    grids.append(ui->grid27);
    grids.append(ui->grid28);

    grids.append(ui->grid30);
    grids.append(ui->grid31);
    grids.append(ui->grid32);
    grids.append(ui->grid33);
    grids.append(ui->grid34);
    grids.append(ui->grid35);
    grids.append(ui->grid36);
    grids.append(ui->grid37);
    grids.append(ui->grid38);

    grids.append(ui->grid40);
    grids.append(ui->grid41);
    grids.append(ui->grid42);
    grids.append(ui->grid43);
    grids.append(ui->grid44);
    grids.append(ui->grid45);
    grids.append(ui->grid46);
    grids.append(ui->grid47);
    grids.append(ui->grid48);

    grids.append(ui->grid50);
    grids.append(ui->grid51);
    grids.append(ui->grid52);
    grids.append(ui->grid53);
    grids.append(ui->grid54);
    grids.append(ui->grid55);
    grids.append(ui->grid56);
    grids.append(ui->grid57);
    grids.append(ui->grid58);

    grids.append(ui->grid60);
    grids.append(ui->grid61);
    grids.append(ui->grid62);
    grids.append(ui->grid63);
    grids.append(ui->grid64);
    grids.append(ui->grid65);
    grids.append(ui->grid66);
    grids.append(ui->grid67);
    grids.append(ui->grid68);

    grids.append(ui->grid70);
    grids.append(ui->grid71);
    grids.append(ui->grid72);
    grids.append(ui->grid73);
    grids.append(ui->grid74);
    grids.append(ui->grid75);
    grids.append(ui->grid76);
    grids.append(ui->grid77);
    grids.append(ui->grid78);

    grids.append(ui->grid80);
    grids.append(ui->grid81);
    grids.append(ui->grid82);
    grids.append(ui->grid83);
    grids.append(ui->grid84);
    grids.append(ui->grid85);
    grids.append(ui->grid86);
    grids.append(ui->grid87);
    grids.append(ui->grid88);
}

void Window::registerGrids()
{
    foreach (GridLabel* grid, grids)
    {
        connect(grid, SIGNAL(focused_grid()),this,SLOT(on_focused_grid()));
    }
}

void Window::on_ExitBtn_clicked()
{
    qApp->quit();
}
