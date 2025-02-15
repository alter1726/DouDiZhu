#include "bgmcontrol.h"
#include "playhand.h"
#include <QAudioOutput>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QTimer>

BGMControl::BGMControl(QObject *parent) : QObject(parent)
{
    for (int i = 0; i < 5; i++) {
        QMediaPlayer *player = new QMediaPlayer(this);
        if (i < 2 || i == 4) {
            player->setLoops(QMediaPlayer::Once);
        } else if (i == 2) {
            player->setLoops(QMediaPlayer::Infinite);
        };
        QAudioOutput *audioOutput = new QAudioOutput;
        player->setAudioOutput(audioOutput);
        audioOutput->setVolume(100);
        m_players.push_back(player);
    }
    initPlayList();
}

void BGMControl::initPlayList()
{
    QStringList list;
    list << "Man"
         << "Woman"
         << "BGM"
         << "Other"
         << "Ending";

    //读json配置文件
    QFile file(":/conf/playList.json");
    file.open(QFile::ReadOnly);
    QByteArray json = file.readAll();
    file.close();
    //解析从文件中读出的json数据
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();

    for (int i = 0; i < list.size(); i++) {
        QString prefix = list.at(i);
        QJsonArray array = obj.value(prefix).toArray();
        QVector<QString> mp3Lists;
        //初始化多媒体播放列表
        for (int j = 0; j < array.size(); j++) {
            mp3Lists.append(array.at(j).toString());
        }
        m_lists.append(mp3Lists);
    }
}

void BGMControl::startBGM(int volume)
{
    m_players[2]->audioOutput()->setVolume(volume);
    m_players[2]->setSource(QUrl(m_lists[2].at(0)));
    m_players[2]->play();
}

void BGMControl::stopBGM()
{
    m_players[2]->stop();
}

//玩家下注了没有?性别?什么时候播放什么样的音频文件
void BGMControl::playerRobLordMusic(int point, RoleSex sex, bool isFirst)
{
    int index = sex == Man ? 0 : 1;
    QString mp3 = QString();
    if (isFirst && point > 0) {
        mp3 = m_lists[index].at(Order);
    } else if (point == 0) {
        if (isFirst) {
            mp3 = m_lists[index].at(NoOrder);
        } else {
            mp3 = m_lists[index].at(NoRob);
        }
    } else if (point == 2) {
        mp3 = m_lists[index].at(Rob1);
    } else if (point == 3) {
        mp3 = m_lists[index].at(Rob2);
    }
    m_players[index]->setSource(QUrl(mp3));
    m_players[index]->play();
}

void BGMControl::playCardMusic(Cards cards, bool isFirst, RoleSex sex)
{
    //得到播放列表
    int index = sex == Man ? 0 : 1;
    QVector<QString> list = m_lists.at(index);
    QString mp3 = QString();

    Card::CardPoint pt = Card::CardPoint::Card_Begin;
    //取出牌型,然后进行判断
    PlayHand hand(cards);
    PlayHand::HandType type = hand.getHandType();
    if (type == PlayHand::Hand_Single || type == PlayHand::Hand_Pair || type == PlayHand::Hand_Triple) {
        pt = cards.getRandomCard().point();
    }
    int number = 0;
    switch (type) {
    case PlayHand::Hand_Single: //单牌
        number = pt - 1;
        break;
    case PlayHand::Hand_Pair: //对牌
        number = pt - 1 + 15;
        break;
    case PlayHand::Hand_Triple: //三张点数相同的牌
        number = pt - 1 + 15 + 13;
        break;
    case PlayHand::Hand_Triple_Single: //三带一
        number = ThreeBindOne;
        break;
    case PlayHand::Hand_Triple_Pair: //三带二
        number = ThreeBindPair;
        break;
    case PlayHand::Hand_Plane:            //飞机
    case PlayHand::Hand_Plane_Two_Single: //飞机带两个单
    case PlayHand::Hand_Plane_Two_Pair:   //飞机带两个对儿
        number = Plane;
        break;
    case PlayHand::Hand_Seq_Pair: //连对
        number = SequencePair;
        break;
    case PlayHand::Hand_Seq_Single: //顺子
        number = Sequence;
        break;
    case PlayHand::Hand_Bomb: //炸弹
        number = Bomb;
        break;
    case PlayHand::Hand_Bomb_Jokers: //王炸
        number = JokerBomb;
        break;
    case PlayHand::Hand_Bomb_Pair:              //炸弹带对
    case PlayHand::Hand_Bomb_Two_Single:        //炸弹带两单
    case PlayHand::Hand_Bomb_Jokers_Pair:       //王炸带对
    case PlayHand::Hand_Bomb_Jokers_Two_Single: //王炸带两单
        number = FourBindTwo;
    default:
        break;
    }

    if (!isFirst && (number >= Plane && number <= FourBindTwo)) {
        mp3 = list.at(MoreBiger1 + QRandomGenerator::global()->bounded(2));
    } else {
        mp3 = list.at(number);
    }
    //播放音乐
    m_players[index]->setSource(QUrl(mp3));
    m_players[index]->play();
    if (number == Bomb || number == JokerBomb) {
        playAssistMusic(BombVoice);
    }
    if (number == Plane) {
        playAssistMusic(PlaneVoice);
    }
}

void BGMControl::playLastMusic(CardType type, RoleSex sex)
{
    //玩家的性别
    int index = sex == Man ? 0 : 1;
    //找到播放列表
    QVector<QString> list = m_lists[index];
    if (m_players[index]->playbackState() == QMediaPlayer::StoppedState) {
        QString mp3 = list.at(type);
        m_players[index]->setSource(QUrl(mp3));
        m_players[index]->play();
    } else {
        QTimer::singleShot(1500, this, [=]() {
            QString mp3 = list.at(type);
            m_players[index]->setSource(QUrl(mp3));
            m_players[index]->play();
        });
    }
}

void BGMControl::playPassMusic(RoleSex sex)
{
    //玩家的性别
    int index = sex == Man ? 0 : 1;
    //找到播放列表
    QVector<QString> list = m_lists[index];
    //找到要播放的音乐
    int random = QRandomGenerator::global()->bounded(4);
    QString mp3 = list.at(Pass1 + random);
    m_players[index]->setSource(QUrl(mp3));
    //播放音乐
    m_players[index]->play();
}

void BGMControl::playAssistMusic(AssistMusic type)
{
    int loopsMode;
    //循环还是单曲播放
    if (type == Dispatch) {
        loopsMode = QMediaPlayer::Infinite;
    } else {
        loopsMode = QMediaPlayer::Once;
    }
    //找到播放列表
    QVector<QString> list = m_lists[3];
    //找到要播放的音乐
    m_players[3]->setSource(list.at(type));
    m_players[3]->setLoops(loopsMode);
    //播放音乐
    m_players[3]->play();
}

void BGMControl::stopAssistMusic()
{
    m_players[3]->stop();
}

void BGMControl::playEndingMusic(bool win)
{
    QString mp3 = QString();
    if (win) {
        mp3 = m_lists[4].at(0);
    } else {
        mp3 = m_lists[4].at(1);
    }
    m_players[4]->setSource(mp3);
    m_players[4]->play();
}
