#ifndef IMPORTANTDAYMANAGER_H
#define IMPORTANTDAYMANAGER_H

#include <QObject>
#include <QDate>
#include <QString>
#include <QList>
#include <QPair>
#include <QSqlDatabase>

// 重要日期管理类
class ImportantDayManager : public QObject {
    Q_OBJECT

public:
    // 构造函数
    explicit ImportantDayManager(QObject *parent = nullptr);

    // 添加重要日期
    bool addDay(const QString &title, const QDate &date);

    // 删除重要日期
    bool removeDay(int id);

    // 更新重要日期
    bool updateDay(int id, const QString &title, const QDate &date);

    // 获取所有重要日期列表
    QList<QPair<int, QString>> listDays();

    // 获取最近的重要日期
    bool nearestDay(QString &title, int &days);

private:
    // 数据库连接对象
    QSqlDatabase m_db;
};

#endif // IMPORTANTDAYMANAGER_H
