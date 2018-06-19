#ifndef GRIDLABEL_H
#define GRIDLABEL_H

#include <QWidget>
#include <QLabel>
#include <QSet>

class GridLabel : public QLabel
{
    Q_OBJECT
public:
    GridLabel(QWidget *parent = nullptr);

    void setEditable(bool);

    void clearNumber();

    void setNumber(int key);

    char getNumber() const;

    void setPotentialNumbers(const QSet<char>& nums);

    QSet<char> getPotentialNumbers() const;

    void remove_digit_from_potentials(char num);
signals:
    void focused_grid();

protected:
    void mousePressEvent(QMouseEvent *ev);

    void mouseReleaseEvent(QMouseEvent *ev);

private:
    int potentialNum(char num, char* temp, size_t sz);

    int notPotentialNum(char num, char* temp, size_t sz);
private:
    bool editable;

    bool definite;

};

#endif // GRIDLABEL_H
