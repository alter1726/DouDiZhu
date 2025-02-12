#ifndef USERPLAYER_H
#define USERPLAYER_H

#include <QObject>
#include "player.h"

class UserPlayer : public Player
{
    Q_OBJECT
public:
    using Player::Player;
    explicit UserPlayer(QObject *parent = nullptr);

    void readyCallLord() override;
    void readyPlayHand() override;
    void thinkCallLord() override;
    void thinkPlayHand() override;
};

#endif // USERPLAYER_H
