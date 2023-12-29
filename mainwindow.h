
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QProcess>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onInputFolderButtonClicked();
    void onOutputFolderButtonClicked();
    void onConvertClicked();

private:
    QLineEdit *inputFolderLineEdit;
    QLineEdit *outputFolderLineEdit;
    QLineEdit *scaleLineEdit;
    QLineEdit *crfLineEdit;
    QComboBox *presetComboBox;
    QPushButton *convertButton;
};

#endif // MAINWINDOW_H
