#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "cards.h"

class Player : public QObject
{
    Q_OBJECT
public:
    enum Role
    {
        Lord,
        Farmer
    }; //角色
    enum Sex
    {
        Man,
        Woman
    }; //性别
    enum Direction
    {
        Left,
        Right
    }; //头像显示方位
    enum Type
    {
        Robot,
        User,
        Unknown
    }; //玩家类型
    explicit Player(QObject *parent = nullptr);
    explicit Player(QString name, QObject *parent = nullptr);

    //名字
    void setName(QString name);
    QString getName();

    //玩家的分数
    void setScore(int score);
    int getScore();

    //角色
    void setRole(Role role);
    Role getRole();

    //性别
    void setSex(Sex sex);
    Sex getSex();

    //方位
    void setDirection(Direction direction);
    Direction getDirection();

    //玩家类型
    void setType(Type type);
    Type getType();

    //游戏结果
    void setWin(bool flag);
    bool win();

    //提供当前对象的上家/下家对象
    void setPrevPlayer(Player *player);
    void setNextPlayer(Player *player);
    Player *getPrevPlayer();
    Player *getNextPlayer();

    //抢地主并下注
    void grabLordBet(int point);

    //存储发牌的时候得到的扑克牌
    void storeDispatchCard(const Card &card);
    void storeDispatchCard(const Cards &cards);

    //得到所有的牌
    Cards getCards();
    //对局结束,清空玩家手中所有的牌
    void clearCards();
    //出牌
    void playHand(const Cards &cards);

    //出牌玩家及打出的牌
    Player *getPendPlayer();
    Cards getPendCards();

    //存储出牌玩家及打出的牌
    void storePendingInfo(Player *player, const Cards &cards);

    virtual void readyCallLord() = 0;
    virtual void readyPlayHand();

signals:
    //通知已经叫地主下注
    void notifyGrabLordBet(Player *player, int bet);

protected:
    QString m_name;
    int m_score;
    Role m_role;
    Sex m_sex;
    //头像显示方位
    Direction m_direction;
    Type m_type;
    bool m_win;
    Player *m_prev = nullptr;
    Player *m_next = nullptr;
    //存储多张扑克牌--地主牌
    Cards m_cards;
    Cards m_pendCards;
    Player *m_pendPlayer = nullptr;
};

#endif // PLAYER_H
