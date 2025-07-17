#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QToolButton>
#include <QPoint>

// 前向声明
class WidgetLayout;
class TipsUpdate;
class WeatherUpdate;
class ImportantDayManager;
class MemoManager;
class QLabel;
class QListWidgetItem;

// 主窗口类
class Widget : public QWidget {
    Q_OBJECT

public:
    // 构造函数
    explicit Widget(QWidget *parent = nullptr);

    // 析构函数
    ~Widget();

protected:
    // 绘制事件
    void paintEvent(QPaintEvent *event) override;

    // 鼠标事件
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // 刷新重要日提示
    void refreshDayTip();

    // 刷新备忘录列表
    void refreshMemo();

    // UI布局
    WidgetLayout *ui;

    // 功能模块
    TipsUpdate *tips;
    WeatherUpdate *weather;
    ImportantDayManager *dayMgr;
    MemoManager *memoMgr;

    // 自定义标题栏
    QWidget *titleBar;
    QToolButton *minBtn;
    QToolButton *closeBtn;

    // 拖动相关
    bool isDragging = false;
    QPoint dragPosition;
};

#endif // WIDGET_H
