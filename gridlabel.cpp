#include "gridlabel.h"

GridLabel::GridLabel(QWidget *parent) : QLabel(parent), editable(false), definite(false)
{
    setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
}

void GridLabel::setEditable(bool e)
{
    editable = e;
}

void GridLabel::clearNumber()
{
    setText("");
}

void GridLabel::setNumber(int key)
{
    //qDebug("grid set number %c", key);
    char c = (char)key;
    char text[64] = {0};
    snprintf(text, sizeof(text), "<font size=\"11\" color=\"blue\">%c</font>", c);
    setText(text);
    setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    definite = true;
    qDebug("setNumber text:%s", text);
}

char GridLabel::getNumber() const
{
    if (!definite) {
        return ' ';
    }

    QByteArray content = text().toLatin1();
    const char* str = content.data();
    //qDebug("getnumber,str=%s",str);

    if (strlen(str) == 0)
        return ' ';
    else {
        char num = ' ';
        const char* pos = strchr(str, '>');
        if (pos != nullptr) {
            while (*pos != '<') {
                if (isdigit(*pos)) {
                    num = *pos;
                    break;
                }
                ++pos;
            }
        }
        return num;
    }
}

void GridLabel::setPotentialNumbers(const QSet<char> &nums)
{
    char text[128] = "<font size=\"3\" color=\"red\">";
    char temp[16] = {0};
    for (char num = '1'; num <= '9'; ++num) {
        if (nums.contains(num)) {
            int sz = potentialNum(num, temp, sizeof(temp));
            strncat(text,temp,sz);
        }
        else {
            int sz = notPotentialNum(num, temp, sizeof(temp));
            strncat(text,temp,sz);
        }
    }
    strncat(text, "</font>", 8);
    setText(text);
    setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    definite = false;
    qDebug("set potentialNumbers:%s",text);
}

QSet<char> GridLabel::getPotentialNumbers() const
{
    if (definite) {
        return QSet<char>();
    }

    QByteArray content = text().toLatin1();
    const char* str = content.data();
    if (strlen(str) == 0)
        return QSet<char>();
    else {
        QSet<char> nums;
        char num = ' ';
        const char* pos = strchr(str, '>');
        if (pos != nullptr) {
            while (*pos != '/') {
                if (isdigit(*pos)) {
                    num = *pos;
                    nums.insert(num);
                }
            }
        }
        return nums;
    }
}

void GridLabel::remove_digit_from_potentials(char num)
{
    qDebug("remove_digit_from_potentials %c",num);
    QString content = text();
    int pos = content.indexOf('>');
    pos = content.indexOf(num, pos);
    content.replace(pos, 1, "&ensp;");
    setText(content);
}

void GridLabel::mousePressEvent(QMouseEvent *ev)
{
    if (editable)
    {
        setStyleSheet("GridLabel {border-radius:2px; border:1px solid #5F92B2;background-color: rgb(200, 215, 232);}");
        emit focused_grid();
    }
}

void GridLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if (editable)
    {
        setStyleSheet("GridLabel {border-radius:2px; border:1px solid #5F92B2;}");
    }
}

int GridLabel::potentialNum(char num, char *temp, size_t sz)
{
    switch (num) {
    case '1':
    case '2':
    case '4':
    case '5':
    case '7':
    case '8':
        snprintf(temp, sz, "%c&ensp;", num);
        break;
    case '3':
    case '6':
        snprintf(temp, sz, "%c<br>", num);
        break;
    case '9':
        snprintf(temp, sz, "%c", num);
        break;
    default:
        break;
    }
    return strlen(temp)+1;
}

int GridLabel::notPotentialNum(char num, char *temp, size_t sz)
{
    switch (num) {
    case '1':
    case '2':
    case '4':
    case '5':
    case '7':
    case '8':
        snprintf(temp, sz, "&ensp;&ensp;");
        break;
    case '3':
    case '6':
        snprintf(temp, sz, "&ensp;<br>");
        break;
    case '9':
        snprintf(temp, sz, "&ensp;");
        break;
    default:
        break;
    }
    return strlen(temp)+1;
}
