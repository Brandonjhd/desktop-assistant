#ifndef WEATHERUPDATE_H
#define WEATHERUPDATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDate>
#include <QDebug>

// 天气更新类
class WeatherUpdate : public QObject {
    Q_OBJECT

public:
    // 构造函数
    explicit WeatherUpdate(QObject *parent = nullptr);

    // 析构函数
    ~WeatherUpdate();

public slots:
    // 启动天气获取
    void fetchWeather();

private slots:
    // 处理实况天气回复
    void baseWeather();

    // 处理预报天气回复
    void allWeather();

private:
    // 检查并发出完成信号
    void checkAndEmit();

    // 网络回复对象
    QNetworkReply *baseReply;
    QNetworkReply *allReply;

    // 网络管理器
    QNetworkAccessManager *manager;

    // 城市信息
    QString name = "金水区";
    QString code = "101180112";
    QString key = "your key !"; //更换为自己的和风天气api key！

    // 天气图标路径
    QString cma = ":/img/img/weather/";

    // 完成标志
    bool baseDone = false;
    bool allDone = false;

    // 当前天气信息
    QString nowInfo;
    QPixmap nowPix;

    // 未来天气信息
    QString futureWeather[4];

signals:
    // 天气数据就绪信号
    void weatherReady(const QString &info, const QPixmap &pix, const QString future[4]);

    // 加载开始信号
    void loadingStarted();
};

#endif // WEATHERUPDATE_H
