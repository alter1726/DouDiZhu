#include "robotgraplord.h"

RobotGrapLord::RobotGrapLord(Player *player, QObject *parent) : QThread(parent)
{
    m_player = player;
}

void RobotGrapLord::run()
{
    msleep(2048);
    m_player->thinkCallLord();
}
