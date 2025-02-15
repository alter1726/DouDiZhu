#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QLabel>
#include <QMainWindow>

#include "animationwindow.h"
#include "bgmcontrol.h"
#include "cardpanel.h"
#include "countdown.h"
#include "gamecontrol.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GamePanel;
}
QT_END_NAMESPACE

class GamePanel : public QMainWindow
{
    Q_OBJECT

public:
    enum AnimationType
    {
        ShunZi,
        LianDui,
        Plane,
        JokerBomb,
        Bomb,
        Bet
    };
    GamePanel(QWidget *parent = nullptr);
    ~GamePanel();

    //初始化游戏控制类信息
    void gameControlInit();
    //更新分数面板的分数
    void updatePlayerScore();
    //切割并存储图片
    void initCardMap();
    //裁剪图片
    void cropImage(QPixmap &pix, int x, int y, Card &c);
    //初始化游戏按钮组
    void initButtonsGroup();
    //初始化玩家在窗口中的上下文环境
    void initPlayerContext();
    //初始化游戏场景
    void initGameScene();
    //处理游戏的状态
    void gameStatusProcess(GameControl::GameStatus status);
    //加载玩家头像
    QPixmap loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role);

    //发牌
    void startDispatchCard();
    //定时器的处理动作
    void onDispatchCard();
    //移动扑克牌
    void cardMoveStep(Player *player, int curPos);
    //处理分发得到的扑克牌
    void disposeCard(Player *player, const Cards &cards);
    //更新扑克牌在窗口中的显示
    void updatePlayerCards(Player *player);
    //处理玩家状态的变化
    void onPlayerStatusChanged(Player *player, GameControl::PlayerStatus status);
    //处理玩家抢地主
    void onGrabLordBet(Player *player, int bet, bool flag);
    //处理玩家的出牌
    void onDisposePlayHand(Player *player, const Cards &cards);
    //处理玩家选牌
    void onCardSelected(Qt::MouseButton button);
    //处理用户玩家出牌
    void onUserPlayHand();
    //用户玩家放弃出牌
    void onUserPass();

    //显示特效动画
    void showAnimation(AnimationType type, int bet = 0);
    //隐藏玩家打出的牌
    void hidePlayerDropCards(Player *player);
    //显示玩家的最终得分
    void showEndingScorePanel();
    //初始化闹钟倒计时
    void initCountDown();

protected:
    void paintEvent(QPaintEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

private:
    enum CardAlign
    {
        Horizontal,
        Vertical
    };
    struct PlayerContext
    {
        //玩家扑克牌显示的区域
        QRect cardRect;
        //出牌的区域
        QRect playHandRect;
        //扑克牌的对齐方式(水平、垂直)
        CardAlign align;
        //扑克牌显示正面还是背面
        bool isFrontSide;
        //游戏过程中的提示信息, 比如不出
        QLabel *info;
        //玩家的头像
        QLabel *roleImg;
        //玩家刚打出的牌
        Cards lastCards;
    };
    Ui::GamePanel *ui;
    QPixmap m_bgImage;
    GameControl *m_gameCtl;
    QVector<Player *> m_playerList;
    QMap<Card, CardPanel *> m_cardMap;
    QSize m_cardSize;
    QPixmap m_cardBackImg;
    QMap<Player *, PlayerContext> m_contextMap;

    CardPanel *m_baseCard;
    CardPanel *m_moveCard;
    QVector<CardPanel *> m_lordCard;
    QPoint m_baseCardPos;
    GameControl::GameStatus m_gameStatus;
    QTimer *m_timer;
    QRect m_cardsRect;
    QHash<CardPanel *, QRect> m_userCards;

    AnimationWindow *m_animation;
    CardPanel *m_curSelCard;
    QSet<CardPanel *> m_selectCards;
    CountDown *m_countDown;
    BGMControl *m_bgm;
};
#endif // GAMEPANEL_H
