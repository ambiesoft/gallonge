#ifndef ALLONGE_H
#define ALLONGE_H

#include <QWidget>

class Allonge : public QWidget
{
    Q_OBJECT
public:
    explicit Allonge(QWidget *parent = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
//    void paintEvent(QPaintEvent *event) override;
//    void resizeEvent(QResizeEvent *event) override;

private:
    QPoint dragPosition;

signals:
};

#endif // ALLONGE_H
