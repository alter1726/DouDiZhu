#include "gamecontrol.h"
#include "playhand.h"
#include <QRandomGenerator>
#include <QTimer>

GameControl::GameControl(QObject *parent) : QObject{ parent }
{
}

void GameControl::initPlayer()
{
    //对象实例化
    m_leftRobot = new Robot("人机A", this);
    m_rightRobot = new Robot("人机B", this);
    m_user = new UserPlayer("本人", this);

    //头像的显示
    m_leftRobot->setDirection(Player::Left);
    m_rightRobot->setDirection(Player::Right);
    m_user->setDirection(Player::Right);

    //性别
    Player::Sex sex;
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_leftRobot->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_rightRobot->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_user->setSex(sex);

    //出牌顺序
    m_user->setPrevPlayer(m_leftRobot);
    m_user->setNextPlayer(m_rightRobot);

    m_leftRobot->setPrevPlayer(m_rightRobot);
    m_leftRobot->setNextPlayer(m_user);

    m_rightRobot->setPrevPlayer(m_user);
    m_rightRobot->setNextPlayer(m_leftRobot);

    //指定当前玩家
    m_currPlayer = m_user;

    //处理玩家发射出的信号
    connect(m_user, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(m_leftRobot, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(m_rightRobot, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);

    //传递出牌玩家对象和玩家打出的牌
    connect(this, &GameControl::pendingInfo, m_leftRobot, &Robot::storePendingInfo);
    connect(this, &GameControl::pendingInfo, m_rightRobot, &Robot::storePendingInfo);
    connect(this, &GameControl::pendingInfo, m_user, &Robot::storePendingInfo);

    //处理玩家出牌
    connect(m_leftRobot, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(m_rightRobot, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(m_user, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
}

Robot *GameControl::getLeftRobot()
{
    return m_leftRobot;
}

Robot *GameControl::getRightRobot()
{
    return m_rightRobot;
}

UserPlayer *GameControl::getUserPlayer()
{
    return m_user;
}

void GameControl::setCurrentPlayer(Player *player)
{
    m_currPlayer = player;
}

Player *GameControl::getCurrentPlayer()
{
    return m_currPlayer;
}

Player *GameControl::getPendPlayer()
{
    return m_pendPlayer;
}

Cards GameControl::getPendCards()
{
    return m_pendCards;
}

void GameControl::initAllCards()
{
    m_allCards.clear();
    for (int p = Card::Card_Begin + 1; p < Card::Card_SJ; p++) {
        for (int s = Card::Suit_Begin + 1; s < Card::Suit_End; s++) {
            Card c((Card::CardPoint)p, (Card::CardSuit)s);
            m_allCards.add(c);
        }
    }
    m_allCards.add(Card(Card::Card_SJ, Card::Suit_Begin));
    m_allCards.add(Card(Card::Card_BJ, Card::Suit_Begin));
}

Card GameControl::takeOneCard()
{
    return m_allCards.getRandomCard();
}

Cards GameControl::getLordCards()
{
    return m_allCards;
}

void GameControl::resetCardData()
{
    //洗牌
    initAllCards();
    //清空所有玩家的牌
    m_leftRobot->clearCards();
    m_rightRobot->clearCards();
    m_user->clearCards();
    //初始化出牌玩家和牌
    m_pendPlayer = nullptr;
    m_pendCards.clear();
}

void GameControl::readyLordCard()
{
    m_currPlayer->readyCallLord();
    emit playerStatusChanged(m_currPlayer, ThinkingForCallLord);
}

void GameControl::becomeLord(Player *player, int bet)
{
    m_curBet = bet;
    player->setRole(Player::Lord);
    player->getPrevPlayer()->setRole(Player::Farmer);
    player->getNextPlayer()->setRole(Player::Farmer);

    m_currPlayer = player;
    player->storeDispatchCard(m_allCards);

    QTimer::singleShot(1024, this, [=]() {
        emit gameStatusChanged(PlayingHand);
        emit playerStatusChanged(player, ThinkingForPlayHand);
        m_currPlayer->readyPlayHand();
    });
}

void GameControl::clearPlayerScore()
{
    m_leftRobot->setScore(0);
    m_rightRobot->setScore(0);
    m_user->setScore(0);
}

int GameControl::getPlayerMaxBet()
{
    return m_betRecord.bet;
}

void GameControl::onGrabBet(Player *player, int bet)
{
    //通知主界面玩家叫地主了(更新信息提示)
    if (bet == 0 || m_betRecord.bet >= bet) {
        emit notifyGrabLordBet(player, 0, false);
    } else if (bet > 0 && m_betRecord.bet == 0) {
        //第一个抢地主的玩家
        emit notifyGrabLordBet(player, bet, true);
    } else {
        //第2、3个抢地主的玩家
        emit notifyGrabLordBet(player, bet, false);
    }

    //判断玩家下注是不是3分, 如果是抢地主结束
    if (bet == 3) {
        //玩家成为地主
        becomeLord(player, bet);
        //清空数据
        m_betRecord.reset();
        return;
    }
    //下注不够3分, 对玩家的分数进行比较, 分数高的是地主
    if (m_betRecord.bet < bet) {
        m_betRecord.bet = bet;
        m_betRecord.player = player;
    }
    m_betRecord.times++;
    //如果每个玩家都抢过一次地主, 抢地主结束
    if (m_betRecord.times == 3) {
        if (m_betRecord.bet == 0) {
            emit gameStatusChanged(DispatchCard);
        } else {
            becomeLord(m_betRecord.player, m_betRecord.bet);
        }
        m_betRecord.reset();
        return;
    }
    //切换玩家, 通知下一个玩家继续抢地主
    m_currPlayer = player->getNextPlayer();
    //发送信号给主界面, 告知当前状态为抢地主
    emit playerStatusChanged(m_currPlayer, ThinkingForCallLord);
    m_currPlayer->readyCallLord();
}

void GameControl::onPlayHand(Player *player, const Cards &card)
{
    //将玩家出牌的信号转发给主界面
    emit notifyPlayHand(player, card);
    //如果不是空牌,给其他玩家发送信号, 保存出牌玩家对象和打出的牌
    if (!card.isEmpty()) {
        m_pendCards = card;
        m_pendPlayer = player;
        emit pendingInfo(player, card);
    }
    //如果有炸弹,低分翻倍
    Cards myCards = const_cast<Cards &>(card);
    PlayHand::HandType type = PlayHand(myCards).getHandType();
    if (type == PlayHand::Hand_Bomb || type == PlayHand::Hand_Bomb_Jokers) {
        m_curBet = m_curBet * 2;
    }

    //如果玩家的牌出完了,计算本局游戏的总分
    if (player->getCards().isEmpty()) {
        Player *prev = player->getPrevPlayer();
        Player *next = player->getNextPlayer();
        if (player->getRole() == Player::Lord) {
            player->setScore(player->getScore() + 2 * m_curBet);
            prev->setScore(prev->getScore() - m_curBet);
            next->setScore(next->getScore() - m_curBet);
            player->setWin(true);
            prev->setWin(false);
            next->setWin(false);
        } else {
            player->setWin(true);
            player->setScore(player->getScore() + m_curBet);
            if (prev->getRole() == Player::Lord) {
                prev->setScore(prev->getScore() - 2 * m_curBet);
                next->setScore(next->getScore() + m_curBet);
                prev->setWin(false);
                next->setWin(true);
            } else {
                next->setScore(next->getScore() - 2 * m_curBet);
                prev->setScore(prev->getScore() + m_curBet);
                next->setWin(false);
                prev->setWin(true);
            }
        }
        emit playerStatusChanged(player, GameControl::Winning);
        return;
    }
    //牌没有出完,下一个玩家继续出牌
    m_currPlayer = player->getNextPlayer();
    m_currPlayer->readyPlayHand();
    emit playerStatusChanged(m_currPlayer, GameControl::ThinkingForPlayHand);
}
