#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>

class MainMenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenuWidget(QWidget *parent = nullptr);

    signals:
        void fileSelected(const QString &filePath); // 文件选择信号

    private slots:
        void openFileDialog(); // 打开文件对话框的槽函数

private:
    QPushButton *openFileButton; // 打开文件的按钮
};

#endif // MAINMENUWIDGET_H
