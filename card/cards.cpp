#include "cards.h"
#include <QRandomGenerator>

Cards::Cards()
{
}

Cards::Cards(const Card &card)
{
    add(card);
}

void Cards::add(const Card &card)
{
    m_cards.insert(card);
}

void Cards::add(const Cards &cards)
{
    m_cards.unite(cards.m_cards);
}

void Cards::add(const QVector<Cards> &cards)
{
}

Cards &Cards::operator<<(const Card &card)
{
    add(card);
    return *this;
}

Cards &Cards::operator<<(const Cards &cards)
{
    add(cards);
    return *this;
}

void Cards::remove(const Card &card)
{
    m_cards.remove(card);
}

void Cards::remove(const Cards &cards)
{
    m_cards.subtract(cards.m_cards);
}

void Cards::remove(const QVector<Cards> &cards)
{
}

Card::CardPoint Cards::maxPoint()
{
    Card::CardPoint max = Card::Card_BJ;
    if (!m_cards.isEmpty()) {
        for (auto it = m_cards.begin(); it != m_cards.end(); it++) {
            if (it->point() > max)
                max = it->point();
        }
    }
    return max;
}

Card::CardPoint Cards::minPoint()
{
    Card::CardPoint min = Card::Card_End;
    if (!m_cards.isEmpty()) {
        for (auto it = m_cards.begin(); it != m_cards.end(); it++) {
            if (it->point() < min)
                min = it->point();
        }
    }
    return min;
}

int Cards::pointCount(Card::CardPoint point)
{
    int count = 0;
    for (auto it = m_cards.begin(); it != m_cards.end(); it++) {
        if (it->point() == point)
            count++;
    }
    return count;
}

bool Cards::contains(const Card &card)
{
    return m_cards.contains(card);
}

bool Cards::contains(const Cards &cards)
{
    return m_cards.contains(cards.m_cards);
}

int Cards::cardCount()
{
    return m_cards.size();
}

bool Cards::isEmpty()
{
    return m_cards.empty();
}

bool Cards::isEmpty() const
{
    return m_cards.empty();
}

void Cards::clear()
{
    m_cards.clear();
}

Card Cards::getRandomCard()
{
    //生成随机数 [0,size)
    int num = QRandomGenerator::global()->bounded(m_cards.size());
    QSet<Card>::const_iterator it = m_cards.constBegin();
    while (num--)
        it++;
    Card card = *it;
    m_cards.erase(it);
    return card;
}

CardList Cards::toCardList(SortType type)
{
    CardList list;
    for (auto it = m_cards.begin(); it != m_cards.end(); it++) {
        list << *it;
    }
    if (type == Asc)
        std::sort(list.begin(), list.end(), lessSort);
    else if (type == Desc)
        std::sort(list.begin(), list.end(), greaterSort);
    return list;
}
