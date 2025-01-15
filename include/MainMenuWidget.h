#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSlider>
#include <QLabel>

class MainMenuWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainMenuWidget(QWidget *parent = nullptr);

    signals:
        void fileSelected(const QString &filePath);

    private slots:
        void openFileDialog();
        void exportFileDialog();
        void scaleChanged(int value);

private:
    QPushButton *openFileButton;
    QPushButton *exportFileButton;
    QSlider *scaleSlider;
    QLabel *scaleLabel;
    QVBoxLayout *mainLayout;
};

#endif // MAINMENUWIDGET_H
