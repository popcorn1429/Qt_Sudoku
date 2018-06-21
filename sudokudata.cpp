#include "sudokudata.h"

SudokuData::SudokuData(QObject *parent) : QObject(parent)
{
    auxiliary.resize(81);
}

void SudokuData::initializeDatem(const QVector<char> &data)
{
    sudoku = data;
    initPotentials();
}

bool SudokuData::validate() const
{
    return true;
}

void SudokuData::solve()
{
    //rule 1
    int ret = rule_one();
    if (ret < 0)
        emit error("rule 1 error");

    if (1 == ret) return;

    //rule 2


}

bool SudokuData::checkComplete() const
{
    for (int i = 0; i < sudoku.size(); ++i) {
        if (sudoku[i] == ' ')
            return false;
    }
    return true;
}

bool SudokuData::initPotentials()
{
    QSet<char> potentials{'1','2','3','4','5','6','7','8','9'};
    for (int i = 0; i < sudoku.size(); ++i) {
        int row = i/9;
        int col = i%9;

        //potentials.clear();
        char num = sudoku[i];
        if (num != ' ') {
            auxiliary[i].clear();
            auxiliary[i].insert(num);
        }
        else {
            auxiliary[i].clear();
            auxiliary[i] = potentials;
            emit potentialInitialized(i, potentials);
        }
    }
}

int SudokuData::rule_one()
{
    int ret = 0;
    for (int i = 0; i < sudoku.size(); ++i) {
        if (sudoku[i] != ' ') {
            ret = remove_potential_digits(i);
            if (ret > 0) {
                return 1;
            }
            else if (ret < 0) {
                return -1;
            }
        }
        else if (auxiliary[i].size() == 1) {
            sudoku[i] = *(auxiliary[i].begin());
            emit update_grid_number(i, sudoku[i]);
            return 1;
        }
    }
    return 0;
}

int SudokuData::remove_potential_digits(int index)
{
    qDebug("remove_potential_digits index:%d",index);
    char c = *(auxiliary[index].begin());

    int row = index/9;
    int col = index%9;
    //same row
    bool modified = false;
    for (int i = 0; i < 9; ++i) {
        if (i == col)
            continue;

        QSet<char> &digits = auxiliary[row*9+i];
        if (digits.find(c) != digits.end()) {
            if (digits.size() == 1)
                return -1;

            digits.remove(c);
            emit remove_digit_from_potentials(row*9+i, c);
            if (!modified)
                modified = true;
        }
    }

    //same col
    for (int i = 0; i < 9; ++i) {
        if (i == row)
            continue;

        QSet<char> &digits = auxiliary[i*9+col];
        if (digits.find(c) != digits.end()) {
            if (digits.size() == 1)
                return -1;

            digits.remove(c);
            emit remove_digit_from_potentials(i*9+col, c);
            if (!modified)
                modified = true;
        }
    }

    //same little_board
    int begin_row = (row / 3) * 3;
    int begin_col = (col / 3) * 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (begin_row + i == row && begin_col + j == col)
                continue;

            QSet<char>& digits = auxiliary[(begin_row+i)*9 + begin_col+j];
            if (digits.find(c) != digits.end()) {
                if (digits.size() == 1)
                    return -1;

                digits.remove(c);
                emit remove_digit_from_potentials((begin_row+i)*9 + begin_col+j, c);
                if (!modified)
                    modified = true;
            }
        }
    }

    return modified ? 1 : 0;
}


