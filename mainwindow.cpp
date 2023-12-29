
#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 创建界面控件
    QLabel *inputFolderLabel = new QLabel("Input Folder:", this);
    inputFolderLineEdit = new QLineEdit(this);
    QPushButton *inputFolderButton = new QPushButton("Browse", this);

    QLabel *outputFolderLabel = new QLabel("Output Folder:", this);
    outputFolderLineEdit = new QLineEdit(this);
    QPushButton *outputFolderButton = new QPushButton("Browse", this);

    QLabel *scaleLabel = new QLabel("Please enter the scale (e.g., 854:480):", this);
    scaleLineEdit = new QLineEdit(this);
    scaleLineEdit->setText("854:480");

    QLabel *crfLabel = new QLabel("Please enter the CRF value(18~28, bigger number smaller file size lower quality):", this);
    crfLineEdit = new QLineEdit(this);
    crfLineEdit->setText("25");

    QLabel *presetLabel = new QLabel("Please select the preset value:", this);
    presetComboBox = new QComboBox(this);
    presetComboBox->addItem("ultrafast");
    presetComboBox->addItem("superfast");
    presetComboBox->addItem("veryfast");
    presetComboBox->addItem("faster");
    presetComboBox->addItem("fast");
    presetComboBox->addItem("medium");
    presetComboBox->addItem("slow");
    presetComboBox->addItem("slower");
    presetComboBox->addItem("veryslow");
    presetComboBox->setCurrentText("veryslow");

    convertButton = new QPushButton("Convert", this);

    // 设置布局
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(inputFolderLabel);
    QHBoxLayout *inputFolderLayout = new QHBoxLayout;
    inputFolderLayout->addWidget(inputFolderLineEdit);
    inputFolderLayout->addWidget(inputFolderButton);
    layout->addLayout(inputFolderLayout);
    layout->addWidget(outputFolderLabel);
    QHBoxLayout *outputFolderLayout = new QHBoxLayout;
    outputFolderLayout->addWidget(outputFolderLineEdit);
    outputFolderLayout->addWidget(outputFolderButton);
    layout->addLayout(outputFolderLayout);
    layout->addWidget(scaleLabel);
    layout->addWidget(scaleLineEdit);
    layout->addWidget(crfLabel);
    layout->addWidget(crfLineEdit);
    layout->addWidget(presetLabel);
    layout->addWidget(presetComboBox);
    layout->addWidget(convertButton);
    setCentralWidget(centralWidget);

    // 连接按钮点击事件到槽函数
    connect(inputFolderButton, &QPushButton::clicked, this, &MainWindow::onInputFolderButtonClicked);
    connect(outputFolderButton, &QPushButton::clicked, this, &MainWindow::onOutputFolderButtonClicked);
    connect(convertButton, &QPushButton::clicked, this, &MainWindow::onConvertClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onInputFolderButtonClicked() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Input Folder");
    if (!folderPath.isEmpty()) {
        inputFolderLineEdit->setText(folderPath);
    }
}

void MainWindow::onOutputFolderButtonClicked() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Output Folder");
    if (!folderPath.isEmpty()) {
        outputFolderLineEdit->setText(folderPath);
    }
}

void MainWindow::onConvertClicked() {
    QString inputFolder = inputFolderLineEdit->text();
    QString outputFolder = outputFolderLineEdit->text();
    QString scale = scaleLineEdit->text();
    QString crf = crfLineEdit->text();
    QString preset = presetComboBox->currentText();

    QString command = "ffmpeg -i \"" + inputFolder + "\" -vf scale=" + scale + " -crf " + crf + " -preset " + preset + " \"" + outputFolder;

    int result = QMessageBox::question(this, "Confirm Conversion", "Command:\n\n" + command + "\n\nDo you want to continue?", QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        QDir inputDir(inputFolder);
        QStringList fileNames = inputDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

        // 创建批处理文件
        QFile batchFile("command.bat");
        if (batchFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&batchFile);

            foreach (const QString &fileName, fileNames) {
                QString inputFile = inputFolder + "/" + fileName;
                QString outputFile = outputFolder + "/" + QFileInfo(fileName).completeBaseName() + ".mp4";

                QString command = "ffmpeg -i \"" + inputFile + "\" -vf scale=" + scale + " -crf " + crf + " -preset " + preset + " \"" + outputFile + "\"";

                qDebug() << "command:" << command;

                stream << command << "\n";
                stream << "ping /n 60 127.0.0.1" << "\n";
            }

            batchFile.close();
        }
    }
}


