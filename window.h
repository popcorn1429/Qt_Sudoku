#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QVector>
#include <QKeyEvent>
#include "sudokudata.h"

class QPushButton;
class QLabel;
class GridLabel;

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

    void clearBoard();

    QVector<char> getDatemFromBoard();

protected:
    void keyPressEvent(QKeyEvent *ev);

private slots:
    void on_potentialInitialized(int index, QSet<char> potentials);

    void on_remove_digit_from_potentials(int index,char num);

    void on_update_grid_number(int index, char num);

    void on_focused_grid();

    void on_importBtn_clicked();

    void on_DiyBtn_clicked(bool checked);

    void on_SolveBtn_clicked();
    void on_ExitBtn_clicked();

private:
    void setGridsEditable();

    void setGridsDiseditable();

    void initGridsContainer();

    void registerGrids();

private:
    Ui::Window *ui;

    QVector<GridLabel*> grids;
    bool                editable;
    GridLabel          *curGrid;

    SudokuData         *data;
};

#endif // WINDOW_H
