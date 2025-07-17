#include "importantdaymanager.h"
#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <climits>

ImportantDayManager::ImportantDayManager(QObject *parent)
    : QObject(parent),
    m_db(DataBaseManager::instance()->db()) {
}

// 添加重要日期
bool ImportantDayManager::addDay(const QString &title, const QDate &date) {
    if (title.trimmed().isEmpty() || !date.isValid()) {
        return false;
    }

    QSqlQuery q(m_db);
    q.prepare("INSERT INTO important_day(title, date) VALUES (?, ?)");
    q.addBindValue(title.trimmed());
    q.addBindValue(date.toString(Qt::ISODate));

    if (!q.exec()) {
        qWarning() << "addDay error:" << q.lastError();
        return false;
    }

    return true;
}

// 删除重要日期
bool ImportantDayManager::removeDay(int id) {
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM important_day WHERE id = ?");
    q.addBindValue(id);

    if (!q.exec()) {
        qWarning() << "removeDay error:" << q.lastError();
        return false;
    }

    return q.numRowsAffected() > 0;
}

// 更新重要日期
bool ImportantDayManager::updateDay(int id, const QString &title, const QDate &date) {
    if (title.trimmed().isEmpty() || !date.isValid()) {
        return false;
    }

    QSqlQuery q(m_db);
    q.prepare("UPDATE important_day SET title = ?, date = ? WHERE id = ?");
    q.addBindValue(title.trimmed());
    q.addBindValue(date.toString(Qt::ISODate));
    q.addBindValue(id);

    if (!q.exec()) {
        qWarning() << "updateDay error:" << q.lastError();
        return false;
    }

    return q.numRowsAffected() > 0;
}

// 获取所有重要日期列表
QList<QPair<int, QString>> ImportantDayManager::listDays() {
    QList<QPair<int, QString>> list;
    QSqlQuery q("SELECT id, title FROM important_day ORDER BY date ASC", m_db);

    while (q.next()) {
        list.append({q.value(0).toInt(), q.value(1).toString()});
    }

    return list;
}

// 获取最近的重要日期
bool ImportantDayManager::nearestDay(QString &title, int &days) {
    title.clear();
    days = 0;

    QSqlQuery q("SELECT title, date FROM important_day", m_db);
    if (q.lastError().isValid()) {
        qWarning() << "nearestDay query error:" << q.lastError();
        return false;
    }

    const QDate today = QDate::currentDate();
    bool found = false;
    int minDiff = INT_MAX;
    QString nearestTitle;

    // 遍历所有重要日期
    while (q.next()) {
        const QString t = q.value(0).toString();
        const QString dStr = q.value(1).toString();
        const QDate rawDate = QDate::fromString(dStr, Qt::ISODate);

        if (!rawDate.isValid()) {
            continue;
        }

        // 计算今年的目标日期
        QDate target = rawDate;
        target.setDate(today.year(), rawDate.month(), rawDate.day());

        // 处理无效日期（如2月29日）
        if (!target.isValid()) {
            target = QDate(today.year(), 3, 1);
        }

        // 如果今年的日期已过，则计算明年的
        if (target < today) {
            target = target.addYears(1);
        }

        // 计算距离天数
        int diff = today.daysTo(target);
        if (diff < minDiff) {
            minDiff = diff;
            nearestTitle = t;
            found = true;
        }
    }

    if (found) {
        title = nearestTitle;
        days = minDiff;
    }

    return found;
}
