#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>
#include "card.h"
#include "player.h"

class CardPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CardPanel(QWidget *parent = nullptr);

    //获取图片的函数 前景及背景
    void setImage(QPixmap &front, QPixmap &back);
    QPixmap getImage();

    //扑克牌显示的面
    void setFrontSide(bool flag);
    bool isFrontSide();

    //记录扑克牌窗口是否选中
    void setSelected(bool flag);
    bool isSelected();

    //获取扑克牌的花色及点数
    void setCard(const Card &card);
    Card getCard();

    //扑克牌拥有者
    void setOwner(Player *player);
    Player *getOwner();

    //扑克牌的点击事件
    void clicked();

protected:
    //绘图事件
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
signals:
    void cardSelected(Qt::MouseButton button);

private:
    QPixmap m_front;
    QPixmap m_back;
    bool m_isFront = true;
    bool m_isSelect = false;
    Card m_card;
    Player *m_owner = nullptr;
};

#endif // CARDPANEL_H
