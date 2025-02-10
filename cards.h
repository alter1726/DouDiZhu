#ifndef CARDS_H
#define CARDS_H
#include "card.h"

#include <QSet>

class Cards
{
public:
    enum SortType
    {
        Asc,
        Desc,
        NoSort
    };
    Cards();
    explicit Cards(const Card &card);

    //发牌,即添加
    void add(const Card &card);
    void add(const Cards &cards);
    void add(const QVector<Cards> &cards);

    //重载<< 一次性插入多个数据
    Cards &operator<<(const Card &card);
    Cards &operator<<(const Cards &cards);

    //出牌,即删除
    void remove(const Card &card);
    void remove(const Cards &cards);
    void remove(const QVector<Cards> &cards);

    //最大点数
    Card::CardPoint maxPoint();
    //最小点数
    Card::CardPoint minPoint();
    //指定点数牌的数量
    int pointCount(Card::CardPoint point);
    //指定的牌是否在集合中
    bool contains(const Card &card);
    bool contains(const Cards &cards);

    //扑克牌数量
    int cardCount();
    //是否为空
    bool isEmpty();
    //清空
    void clear();

    //随机取牌
    Card getRandomCard();

    // QSet -> QVector
    CardList toCardList(SortType type = Desc);

private:
    QSet<Card> m_cards;
};

#endif // CARDS_H
