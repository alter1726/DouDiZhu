#include "userplayer.h"

UserPlayer::UserPlayer(QObject *parent) : Player{ parent }
{
    m_type = Player::User;
}

void UserPlayer::readyCallLord()
{
}

void UserPlayer::readyPlayHand()
{
    emit startCountDown();
}

void UserPlayer::thinkCallLord()
{

}

void UserPlayer::thinkPlayHand()
{

}
