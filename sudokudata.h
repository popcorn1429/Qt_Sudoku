#ifndef SUDOKUDATA_H
#define SUDOKUDATA_H

#include <QObject>
#include <QVector>
#include <QSet>

enum class GridState {
    Originally_Valued,
    Solved,
    Guess,
    Conflict
};

class SudokuData : public QObject
{
    Q_OBJECT
public:
    explicit SudokuData(QObject *parent = nullptr);

    void initialize_by_file(const QString& fileName);
    void initialize_by_window(const QVector<char>& data);

    bool validate() const;

    void solve();
signals:
    void potentialInitialized(int index, QSet<char> potentials);

    void dataChanged(int index, GridState state, char c, QSet<char> potentials);

    void error(QString);

    void remove_digit_from_potentials(int index, char c);

    void update_grid_number(int index, char c);

public slots:

private:
    bool initPotentials();

    int rule_one();

    int remove_potential_digits(int index);

private:
    QVector<char>       sudoku;
    QVector<QSet<char>> auxiliary;
};

#endif // SUDOKUDATA_H
