#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>
#include "cards.h"
#include "robot.h"
#include "userplayer.h"

class GameControl : public QObject
{
    Q_OBJECT
public:
    //游戏状态
    enum GameStatus
    {
        DispatchCard,
        CallingLord,
        PlayingHand
    };
    //玩家状态
    enum PlayerStatus
    {
        ThinkingForCallLord,
        ThinkingForPlayHand,
        Winning
    };
    explicit GameControl(QObject *parent = nullptr);

    //初始化玩家
    void initPlayer();

    //得到玩家的实例对象
    Robot *getLeftRobot();
    Robot *getRightRobot();
    UserPlayer *getUserPlayer();

    void setCurrentPlayer(Player *player);
    Player *getCurrentPlayer();
    //得到出牌玩家和打出的牌
    Player *getPendPlayer();
    Cards getPendCards();

    //初始化卡牌数据
    void initAllCards();
    //每次发一张牌
    Card takeOneCard();
    //拿地主牌
    Cards getLordCards();
    //重置卡牌数据
    void resetCardData();

    //准备叫地主
    void readyLordCard();
    //成为地主
    void becomeLord(Player *player, int bet);
    //清空所有玩家的得分
    void clearPlayerScore();
    //得到玩家下注的最高分数
    int getPlayerMaxBet();

    //处理叫地主
    void onGrabBet(Player *player, int bet);

signals:
    void playerStatusChanged(Player *player, PlayerStatus status);
    //通知玩家抢地主
    void notifyGrabLordBet(Player *player, int bet, bool flag);
    //游戏状态变化
    void gameStatusChanged(GameStatus status);

private:
    struct BetRecord
    {
        BetRecord() { reset(); }
        void reset()
        {
            player = nullptr;
            bet = 0;
            times = 0;
        }
        Player *player;
        int bet;
        int times; //第几次叫地主
    };
    Robot *m_leftRobot = nullptr;
    Robot *m_rightRobot = nullptr;
    UserPlayer *m_user = nullptr;
    Player *m_currPlayer = nullptr;
    Player *m_pendPlayer = nullptr;
    Cards m_pendCards;
    Cards m_allCards;
    BetRecord m_betRecord;
    int m_curBet = 0;
};

#endif // GAMECONTROL_H
