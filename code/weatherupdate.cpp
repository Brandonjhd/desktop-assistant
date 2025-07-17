#include "weatherupdate.h"

WeatherUpdate::WeatherUpdate(QObject *parent) : QObject(parent) {}

WeatherUpdate::~WeatherUpdate() {}

// 启动天气获取
void WeatherUpdate::fetchWeather() {
    emit loadingStarted();

    // 初始化网络管理器
    manager = new QNetworkAccessManager(this);
    baseDone = false;
    allDone = false;

    // 创建实况天气请求
    QNetworkRequest baseRequest(QUrl("https://mu759fccxy.re.qweatherapi.com/v7/weather/now?location="
                                     + code + "&key=" + key));
    baseReply = manager->get(baseRequest);
    connect(baseReply, &QNetworkReply::finished, this, &WeatherUpdate::baseWeather);

    // 创建预报天气请求
    QNetworkRequest allRequest(QUrl("https://mu759fccxy.re.qweatherapi.com/v7/weather/7d?location="
                                    + code + "&key=" + key));
    allReply = manager->get(allRequest);
    connect(allReply, &QNetworkReply::finished, this, &WeatherUpdate::allWeather);
}

// 处理实况天气回复
void WeatherUpdate::baseWeather() {
    if (baseReply->error() != QNetworkReply::NoError) {
        nowInfo = "网络错误: " + baseReply->errorString();
        nowPix = QPixmap();
        baseReply->deleteLater();
        baseDone = true;
        checkAndEmit();
        return;
    }

    // 读取响应数据
    QByteArray baseData = baseReply->readAll();
    baseReply->deleteLater();

    // 解析JSON数据
    QJsonDocument baseDoc = QJsonDocument::fromJson(baseData);
    if (!baseDoc.isObject()) {
        nowInfo = "无法解析当前天气数据！";
        nowPix = QPixmap();
        baseDone = true;
        checkAndEmit();
        return;
    }

    // 提取天气数据
    QJsonObject baseObj = baseDoc.object();
    QJsonObject live = baseObj["now"].toObject();

    QString baseWeather = live["text"].toString();
    QString baseTemperature = live["temp"].toString();
    QString baseWinddirection = live["windDir"].toString();
    QString baseWindpower = live["windScale"].toString();
    QString baseHumidity = live["humidity"].toString();

    // 处理时间格式
    QString rawTime = live["obsTime"].toString();
    QDateTime dt = QDateTime::fromString(rawTime, Qt::ISODateWithMs);
    QString baseReporttime = dt.toString("MM-dd hh:mm");

    // 组装天气信息
    nowInfo = QString("天气：" + baseWeather + "\n气温：" + baseTemperature
                      + "℃\n风向：" + baseWinddirection + "\n风速：" + baseWindpower
                      + "级\n湿度：" + baseHumidity + "%\n更新时间：" + baseReporttime);

    // 根据天气类型设置图标
    QString imgUrl;
    bool hasIcon = true;

    if (baseWeather == "晴") imgUrl = cma + "0.png";
    else if (baseWeather == "多云") imgUrl = cma + "1.png";
    else if (baseWeather == "阴") imgUrl = cma + "2.png";
    else if (baseWeather == "阵雨") imgUrl = cma + "3.png";
    else if (baseWeather == "雷阵雨") imgUrl = cma + "4.png";
    else if (baseWeather == "雷雨") imgUrl = cma + "5.png";
    else if (baseWeather == "雨加雪") imgUrl = cma + "6.png";
    else if (baseWeather == "小雨") imgUrl = cma + "7.png";
    else if (baseWeather == "中雨" || baseWeather == "小到中雨") imgUrl = cma + "8.png";
    else if (baseWeather == "大雨") imgUrl = cma + "9.png";
    else if (baseWeather == "暴雨" || baseWeather == "大暴雨" || baseWeather == "特大暴雨") imgUrl = cma + "10.png";
    else if (baseWeather == "小雪") imgUrl = cma + "14.png";
    else if (baseWeather == "中雪") imgUrl = cma + "15.png";
    else if (baseWeather == "大雪") imgUrl = cma + "16.png";
    else if (baseWeather == "暴雪") imgUrl = cma + "17.png";
    else if (baseWeather == "雾") imgUrl = cma + "18.png";
    else if (baseWeather == "夜间阵雨") imgUrl = cma + "36.png";
    else hasIcon = false;

    // 加载天气图标
    if (hasIcon) {
        nowPix = QPixmap(imgUrl);
        baseDone = true;
        checkAndEmit();
    } else {
        nowPix = QPixmap();
        baseDone = true;
        checkAndEmit();
    }
}

// 处理预报天气回复
void WeatherUpdate::allWeather() {
    if (allReply->error() != QNetworkReply::NoError) {
        for (int i = 0; i < 4; ++i) {
            futureWeather[i] = "网络错误: " + allReply->errorString();
        }
        allReply->deleteLater();
        allDone = true;
        checkAndEmit();
        return;
    }

    // 读取响应数据
    QByteArray allData = allReply->readAll();
    allReply->deleteLater();

    // 解析JSON数据
    QJsonDocument allDoc = QJsonDocument::fromJson(allData);
    if (!allDoc.isObject()) {
        for (int i = 0; i < 4; ++i) {
            futureWeather[i] = "无法解析预报数据！";
        }
        allDone = true;
        checkAndEmit();
        return;
    }

    // 提取预报数据
    QJsonObject allObj = allDoc.object();
    QJsonArray dailyArray = allObj["daily"].toArray();

    if (dailyArray.size() < 4) {
        for (int i = 0; i < 4; ++i) {
            futureWeather[i] = "数据不足！";
        }
        allDone = true;
        checkAndEmit();
        return;
    }

    // 处理4天预报数据
    for (int i = 0; i < 4; ++i) {
        QJsonObject casts = dailyArray.at(i).toObject();

        // 计算星期
        int allWeek = (QDate::currentDate().dayOfWeek() + i) % 7;
        if (allWeek == 0) allWeek = 7;

        QString weekday;
        if (allWeek == 1) weekday = "星期一";
        else if (allWeek == 2) weekday = "星期二";
        else if (allWeek == 3) weekday = "星期三";
        else if (allWeek == 4) weekday = "星期四";
        else if (allWeek == 5) weekday = "星期五";
        else if (allWeek == 6) weekday = "星期六";
        else if (allWeek == 7) weekday = "星期天";

        // 提取天气信息
        QString allDayweather = casts["textDay"].toString();
        QString allDaytemp = casts["tempMax"].toString();
        QString allNightweather = casts["textNight"].toString();
        QString allNighttemp = casts["tempMin"].toString();

        if (allDayweather.isEmpty() || allDaytemp.isEmpty() || allNightweather.isEmpty() || allNighttemp.isEmpty()) {
            futureWeather[i] = "数据缺失！";
            continue;
        }

        // 组装预报信息
        futureWeather[i] = QString(weekday + "\n日间：" + allDayweather + "\n气温：" + allDaytemp
                                   + "℃\n夜间：" + allNightweather + "\n气温：" + allNighttemp + "℃");
    }

    allDone = true;
    checkAndEmit();
}

// 检查并发出完成信号
void WeatherUpdate::checkAndEmit() {
    if (baseDone && allDone) {
        emit weatherReady(nowInfo, nowPix, futureWeather);
    }
}
