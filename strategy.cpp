#include "strategy.h"

Strategy::Strategy(Player *player, const Cards &cards) : m_player(player), m_cards(cards)
{
}

Cards Strategy::findSamePointCards(Card::CardPoint point, int count)
{
    if (count < 1 || count > 4) {
        return Cards();
    }
    //大小王
    if (point == Card::Card_SJ || point == Card::Card_BJ) {
        //大小王只有一张
        if (count > 1) {
            return Cards();
        }

        Card card;
        card.setPoint(point);
        card.setSuit(Card::Suit_Begin);
        if (m_cards.contains(card)) {
            Cards cards;
            cards.add(card);
            return cards;
        }
        return Cards();
    }

    //不是大小王
    int findCount = 0;
    Cards findCards;
    for (int suit = Card::Suit_Begin + 1; suit < Card::Suit_End; suit++) {
        Card card;
        card.setPoint(point);
        card.setSuit((Card::CardSuit)suit);
        if (m_cards.contains(card)) {
            findCount++;
            findCards.add(card);
            if (findCount == count) {
                return findCards;
            }
        }
    }
    return Cards();
}

QVector<Cards> Strategy::findCardsByCount(int count)
{
    if (count < 1 || count > 4) {
        return QVector<Cards>();
    }

    QVector<Cards> cardsArray;
    for (Card::CardPoint point = Card::Card_3; point < Card::Card_End; point = (Card::CardPoint)(point + 1)) {
        if (m_cards.pointCount(point) == count) {
            Cards cs;
            cs << findSamePointCards(point, count);
            cardsArray << cs;
        }
    }
    return cardsArray;
}

Cards Strategy::getRangeCards(Card::CardPoint begin, Card::CardPoint end)
{
    Cards rangeCards;
    for (Card::CardPoint point = begin; point < end; point = (Card::CardPoint)(point + 1)) {
        int count = m_cards.pointCount(point);
        Cards cs = findSamePointCards(point, count);
        rangeCards << cs;
    }
    return rangeCards;
}

QVector<Cards> Strategy::findCardType(PlayHand hand, bool beat)
{
    PlayHand::HandType type = hand.getHandType();
    Card::CardPoint point = hand.getCardPoint();
    int extra = hand.getExtra();
    //确定起始点数
    Card::CardPoint beginPoint = beat ? Card::CardPoint(point + 1) : Card::Card_3;

    switch (type) {
    case PlayHand::Hand_Single:
        return getCards(beginPoint, 1);
    case PlayHand::Hand_Pair:
        return getCards(beginPoint, 2);
    case PlayHand::Hand_Triple:
        return getCards(beginPoint, 3);
    case PlayHand::Hand_Triple_Single:
        return getTripleSingleOrPair(beginPoint, PlayHand::Hand_Single);
    case PlayHand::Hand_Triple_Pair:
        return getTripleSingleOrPair(beginPoint, PlayHand::Hand_Pair);
    case PlayHand::Hand_Plane:
        return getPlane(beginPoint);
    case PlayHand::Hand_Plane_Two_Single:
        return getPlane2SingleOr2Pair(beginPoint, PlayHand::Hand_Single);
    case PlayHand::Hand_Plane_Two_Pair:
        return getPlane2SingleOr2Pair(beginPoint, PlayHand::Hand_Pair);
    case PlayHand::Hand_Seq_Pair: {
        CardInfo info;
        info.begin = beginPoint;
        info.end = Card::Card_Q;
        info.number = 2;
        info.base = 3;
        info.extra = extra;
        info.beat = beat;
        info.getSeq = &Strategy::getBaseSeqPair;
        return getSepPairOrSeqSingle(info);
    }
    case PlayHand::Hand_Seq_Single: {
        CardInfo info;
        info.begin = beginPoint;
        info.end = Card::Card_10;
        info.number = 1;
        info.base = 5;
        info.extra = extra;
        info.beat = beat;
        info.getSeq = &Strategy::getBaseSeqSingle;
        return getSepPairOrSeqSingle(info);
    }
    case PlayHand::Hand_Bomb:
        return getBomb(beginPoint);
    default:
        return QVector<Cards>();
    }
}

QVector<Cards> Strategy::getCards(Card::CardPoint point, int number)
{
    QVector<Cards> findCardsArray;
    for (Card::CardPoint pt = point; pt < Card::Card_End; pt = (Card::CardPoint)(pt + 1)) {
        //尽量不拆分别的牌型
        if (m_cards.pointCount(pt) == number) {
            Cards cs = findSamePointCards(pt, number);
            findCardsArray << cs;
        }
    }
    return findCardsArray;
}

QVector<Cards> Strategy::getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type)
{
    //找到点数相同的三张牌
    QVector<Cards> findCardArray = getCards(begin, 3);
    if (!findCardArray.isEmpty()) {
        //将找到的牌从用户手中删除
        Cards remainCards = m_cards;
        remainCards.remove(findCardArray);
        //搜索单牌或者成对的牌
        Strategy st(m_player, remainCards);
        QVector<Cards> cardsArray = st.findCardType(PlayHand(type, Card::Card_Begin, 0), false);
        if (!cardsArray.isEmpty()) {
            //将找到的牌和三张点数相同的牌进行组合
            for (int i = 0; i < findCardArray.size(); i++) {
                findCardArray[i].add(cardsArray.at(i));
            }
        } else {
            findCardArray.clear();
        }
    }
    return findCardArray;
}

QVector<Cards> Strategy::getPlane(Card::CardPoint begin)
{
    QVector<Cards> findCardArray;
    for (Card::CardPoint point = begin; point <= Card::Card_K; point = (Card::CardPoint)(point + 1)) {
        //根据点数和数量进行搜索
        Cards prevCards = findSamePointCards(point, 3);
        Cards nextCards = findSamePointCards((Card::CardPoint)(point + 1), 3);
        if (!prevCards.isEmpty() && !nextCards.isEmpty()) {
            Cards tmp;
            tmp << prevCards << nextCards;
            findCardArray << tmp;
        }
    }
    return findCardArray;
}

QVector<Cards> Strategy::getPlane2SingleOr2Pair(Card::CardPoint begin, PlayHand::HandType type)
{
    //找飞机
    QVector<Cards> findCardArray = getPlane(begin);
    if (!findCardArray.isEmpty()) {
        //将找到的牌从用户手中删除
        Cards remainCards = m_cards;
        remainCards.remove(findCardArray);
        //搜索单牌或者成对的牌
        Strategy st(m_player, remainCards);
        QVector<Cards> cardsArray = st.findCardType(PlayHand(type, Card::Card_Begin, 0), false);
        if (cardsArray.size() >= 2) {
            //找到了则将其添加到飞机组合中
            for (int i = 0; i < findCardArray.size(); i++) {
                Cards tmp;
                tmp << cardsArray[0] << cardsArray[1];
                findCardArray[i].add(tmp);
            }
        } else {
            findCardArray.clear();
        }
    }
    return findCardArray;
}

QVector<Cards> Strategy::getSepPairOrSeqSingle(CardInfo &info)
{
    QVector<Cards> findCardsArray;
    if (info.beat) {
        //最少3个, 最大A
        for (Card::CardPoint point = info.begin; point <= info.end; point = (Card::CardPoint)(point + 1)) {
            bool found = true;
            Cards seqCards;
            for (int i = 0; i < info.extra; i++) {
                //基于点数和数量进行牌的搜索
                Cards cards = findSamePointCards((Card::CardPoint)(point + i), info.number);
                if (cards.isEmpty() || (point + info.extra >= Card::Card_2)) {
                    found = false;
                    seqCards.clear();
                    break;
                } else {
                    seqCards << cards;
                }
            }
            if (found) {
                findCardsArray << seqCards;
                return findCardsArray;
            }
        }
    } else {
        for (Card::CardPoint point = info.begin; point <= info.end; point = (Card::CardPoint)(point + 1)) {
            //将找到的这个基础连对存储起来
            Cards baseSeq = (this->*info.getSeq)(point);
            if (baseSeq.isEmpty()) {
                continue;
            }
            //连对存储到容器中
            findCardsArray << baseSeq;

            int followed = info.base;
            Cards alreadyFollowedCards; //存储后续找到的满足条件的连对
            while (true) {
                //新的起始点数
                Card::CardPoint followedPoint = Card::CardPoint(point + followed);
                //判断是否超出了上限
                if (followedPoint >= Card::Card_2) {
                    break;
                }
                Cards follwedCards = findSamePointCards(followedPoint, info.number);
                if (follwedCards.isEmpty()) {
                    break;
                } else {
                    alreadyFollowedCards << follwedCards;
                    Cards newSeq = baseSeq;
                    newSeq << alreadyFollowedCards;
                    findCardsArray << newSeq;
                    followed++;
                }
            }
        }
    }
    return findCardsArray;
}

Cards Strategy::getBaseSeqPair(Card::CardPoint point)
{
    Cards cards0 = findSamePointCards(point, 2);
    Cards cards1 = findSamePointCards((Card::CardPoint)(point + 1), 2);
    Cards cards2 = findSamePointCards((Card::CardPoint)(point + 2), 2);
    Cards baseSeq;
    if (!cards0.isEmpty() && !cards1.isEmpty() && !cards2.isEmpty()) {
        baseSeq << cards0 << cards1 << cards2;
    }
    return baseSeq;
}

Cards Strategy::getBaseSeqSingle(Card::CardPoint point)
{
    Cards cards0 = findSamePointCards(point, 1);
    Cards cards1 = findSamePointCards((Card::CardPoint)(point + 1), 1);
    Cards cards2 = findSamePointCards((Card::CardPoint)(point + 2), 1);
    Cards cards3 = findSamePointCards((Card::CardPoint)(point + 3), 1);
    Cards cards4 = findSamePointCards((Card::CardPoint)(point + 4), 1);
    Cards baseSeq;
    if (!cards0.isEmpty() && !cards1.isEmpty() && !cards2.isEmpty() && !cards3.isEmpty() && !cards4.isEmpty()) {
        baseSeq << cards0 << cards1 << cards2 << cards3 << cards4;
    }
    return baseSeq;
}

QVector<Cards> Strategy::getBomb(Card::CardPoint begin)
{
    QVector<Cards> findcardsArray;
    for (Card::CardPoint point = begin; point < Card::Card_End; point = (Card::CardPoint)(point + 1)) {
        Cards cs = findSamePointCards(point, 4);
        if (!cs.isEmpty()) {
            findcardsArray << cs;
        }
    }
    return findcardsArray;
}
