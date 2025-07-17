#ifndef WIDGETLAYOUT_H
#define WIDGETLAYOUT_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QListWidget>
#include <QStackedLayout>
#include <QSpacerItem>
#include <QPixmap>

// 主窗口布局类
class WidgetLayout : public QWidget {
    Q_OBJECT

public:
    // 构造函数
    explicit WidgetLayout(QWidget *parent = nullptr);

    // 获取组件访问器
    QToolButton* getUpDateBtn() const { return updatebtn; }
    QToolButton* getTipsUpDateBtn() const { return tipsupdatebtn; }
    QLabel* getTipsLbl() const { return tipslbl; }
    QLabel* getImportantLbl() const { return importantlbl; }
    QToolButton* getAddDayBtn() const { return adddaybtn; }
    QToolButton* getAddMemoBtn() const { return addmemobtn; }
    QListWidget* getMemoTable() const { return memoTable; }

public slots:
    // 设置天气信息
    void setWeatherInfo(const QString &info, const QPixmap &pix, const QString infos[4]);

    // 显示天气加载状态
    void showLoading();

    // 设置Tips信息
    void setTipsInfo(const QPixmap &pix, const QString &text);

    // 显示Tips加载状态
    void showTipsLoading();

    // 设置Tips文字间距
    void setTipsTextSpacing(int spacing);

private:
    // 天气组件
    QLabel *nowlbl;
    QLabel *nowimglbl;
    QLabel *futurelbl_1;
    QLabel *futurelbl_2;
    QLabel *futurelbl_3;
    QLabel *futurelbl_4;
    QToolButton *updatebtn;
    QHBoxLayout *nowlot;
    QHBoxLayout *futurelot;
    QVBoxLayout *weatherlot;
    QFrame *weatherFrame;

    // Tips组件
    QLabel *tipsimglbl;
    QLabel *tipslbl;
    QLabel *importantlbl;
    QToolButton *tipsupdatebtn;
    QToolButton *adddaybtn;
    QVBoxLayout *tipsVlot;
    QFrame *tipsFrame;
    QStackedLayout *tipsStack;
    QSpacerItem *tipsTextSpacer;

    // 备忘录组件
    QListWidget *memoTable;
    QToolButton *addmemobtn;
    QVBoxLayout *memolot;
    QFrame *memoFrame;

    // 主布局
    QVBoxLayout *mainlot;
};

#endif // WIDGETLAYOUT_H
