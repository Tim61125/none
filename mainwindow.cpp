#include "mainwindow.h"
#include"second_window.h"
#include "samples/background.h"
#include "samples/customtiles.h"
#include "samples/flags.h"
#include "samples/items.h"
#include "samples/mouse.h"
#include "samples/utilities.h"
#include "samples/widgets.h"
#include "samples/placemark.h"
#include "ui_mainwindow.h"
#include <QtLocation>
#include <QAction>
#include <QClipboard>
#include <QDir>
#include <QGVDrawItem.h>
#include <QQuickView>
#include <QMessageBox>
#include "on_map_click.h"
#include "QGVMap.h"
#include "QGVImage.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QGVLayer.h>
#include <QPainter>
#include <QGVLayer.h>
#include"samples/placemark.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QCamera>
#include <QVideoWidget>
#include <QQuickWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include "qbatterywidget.h"
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : ui(new Ui::MainWindow)


{
    ui->setupUi(this);

    QQuickWidget *camera = new QQuickWidget;
    camera->setSource(QUrl("qrc:/camera.qml"));
    ui->horizontalLayout->addWidget(camera);

    QMetaObject::invokeMethod(this, "init", Qt::QueuedConnection);
    //connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::on_pushButton_8_clicked);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &MainWindow::on_pushButton_9_clicked);


}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{

    QDir("cacheDir").removeRecursively();
    mCache = new QNetworkDiskCache(this);
    mCache->setCacheDirectory("cacheDir");
    mManager = new QNetworkAccessManager(this);
    mManager->setCache(mCache);
    QGV::setNetworkManager(mManager);
    DemoItem* item = new BackgroundDemo(ui->geoMap, this);
    DemoItem* mouseItem = new MouseDemo(ui->geoMap, this);
    const auto target =
        QGV::GeoRect(QGV::GeoPos(41.988577, 74.964334), QGV::GeoPos(43.888577, 76.964334));

    item->init();
    mouseItem->init();
    ui->geoMap->cameraTo(QGVCameraActions(ui->geoMap).scaleTo(target));

    drawingLayer= new QGVLayer();
    drawingLayer->setName("Route layer");
    drawingLayer->setDescription("");
    drawingLayer->bringToFront();
    drawingLayer->setVisible(true);

    ui->geoMap->addItem(drawingLayer);

    sloi= new QGVLayer();
    sloi->setName("Route layer");
    sloi->setDescription("");
    sloi->bringToFront();
    sloi->setVisible(true);

    ui->geoMap->addItem(sloi);
    connect(ui->geoMap, static_cast<void (QGVMap::*)(QPointF)>(&QGVMap::mapMousePress), this, &MainWindow::mousePress);
}
void MainWindow::on_pushButton_2_clicked()
{

    mediaPlayer = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl::fromLocalFile("C:/Users/acer/Documents/NanoSpace/fly.mp3"));
    audioOutput->setVolume(50);
    mediaPlayer->play();

}


void MainWindow::on_pushButton_4_clicked()
{
    if(!drawingRoute)
    {
        drawingRoute = true;
        pendingRoute = new QRoute();
        pendingRoute->setVisible(true);
        drawingLayer->addItem(pendingRoute);


    }
    else
        drawingRoute = false;
    mediaPlayer = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    mediaPlayer->setAudioOutput(audioOutput);
    // ...
    mediaPlayer->setSource(QUrl::fromLocalFile("C:/Users/acer/Documents/NanoSpace/mission.mp3"));
    audioOutput->setVolume(50);
    mediaPlayer->play();
}


void MainWindow::mousePress(QPointF coords) {
    if(drawingRoute)
    {
        auto base =  ui->geoMap->getProjection()->projToGeo(coords);
        QPointF realCoords = {base.latitude(), base.longitude()};
        pendingRoute->addPoint(realCoords);
        auto based = ui->geoMap->getProjection()->projToGeo(coords);
        Placemark *placemark = new Placemark(QGV::GeoPos(based.latitude(), based.longitude()));
        drawingLayer->addItem(placemark);
        on_pushButton_clicked(coords);
        qInfo() << "Added point: " << realCoords;
        update();
    }
    else if (addMarkersEnabled) { // Если флаг разрешает добавление меток
        auto basede = ui->geoMap->getProjection()->projToGeo(coords);
        Placemark *placemark = new Placemark(QGV::GeoPos(basede.latitude(), basede.longitude()));
        sloi->addItem(placemark);
        update();
    }
    }



    void MainWindow::on_pushButton_5_clicked()
    {
    if (!sloiy) {
        sloiy = true;
        pending = new QRoute();
        pending->setVisible(true);
        sloi->addItem(pending);
        addMarkersEnabled = true; // Установка флага для разрешения добавления меток
    }
    else {
        sloiy = false;
        addMarkersEnabled = false; // Сброс флага
    }
    }


void MainWindow::on_pushButton_3_clicked()
{
     mediaPlayer = new QMediaPlayer;
     audioOutput = new QAudioOutput;
     mediaPlayer->setAudioOutput(audioOutput);
     // ...
     mediaPlayer->setSource(QUrl::fromLocalFile("C:/Users/acer/Documents/NanoSpace/posad.mp3"));
     audioOutput->setVolume(50);
     mediaPlayer->play();

}



void MainWindow::on_pushButton_8_clicked()
{
     // Очищаем verticalLayout
     QLayoutItem* item;
     while ((item = ui->verticalLayout->takeAt(0)) != nullptr)
     {
        delete item->widget();
        delete item;
     }

     // Добавляем новые объекты
     ui->verticalLayout->addWidget(new QPushButton("Новая кнопка"));
     ui->verticalLayout->addWidget(new QCheckBox("Новый флажок"));

     // Добавляем новый QLabel и QLineEdit
     QLabel* coordLabel = new QLabel("Координаты:");
     QLineEdit* coordEdit = new QLineEdit();
     coordEdit->setObjectName("coordEdit"); // уникальный идентификатор

     // Добавляем QLabel и QLineEdit в horizontalLayout
     QHBoxLayout* horizontalLayout = new QHBoxLayout();
     horizontalLayout->addWidget(coordLabel);
     horizontalLayout->addWidget(coordEdit);
     ui->verticalLayout->addLayout(horizontalLayout);
}

void MainWindow::on_pushButton_9_clicked()
{
     // Очищаем verticalLayout
     QLayoutItem* item;
     while ((item = ui->verticalLayout->takeAt(0)) != nullptr)
     {
        delete item->widget();
        delete item;
     }

     // Добавляем новые объекты
     ui->verticalLayout->addWidget(new QComboBox());
     clearMap();
}

void MainWindow::on_pushButton_10_clicked()
{
     Second_window* secondWindow = new Second_window(); // Создание экземпляра класса `Second_window`
     secondWindow->showFullScreen(); // Открытие окна в полноэкранном режиме
}



void MainWindow::on_pushButton_clicked(QPointF coords)
{
    // Создать новый QLabel
    QLabel* coordinatesLabel = new QLabel(this);

    // Получить координаты из последнего события нажатия мыши на карту
    auto basede = ui->geoMap->getProjection()->projToGeo(coords);
    // Преобразовать координаты в строку
    QString coordsString = QString("Координаты: %1, %2").arg(basede.latitude()).arg(basede.longitude());

    // Установить текст в QLabel
    coordinatesLabel->setText(coordsString);

    // Добавить QLabel в verticalLayout
    ui->verticalLayout->addWidget(coordinatesLabel);
}

void MainWindow::clearMap()
{
    // Удаление всех маршрутов из слоя `drawingLayer`
    QList<QGraphicsItem*> drawingItems = drawingLayer->mItems;
    for (QGraphicsItem* item : drawingItems)
    {
        QGVItem* qgvItem = dynamic_cast<QGVItem*>(item);
        if (qgvItem)
        {
            drawingLayer->removeItem(qgvItem);
            delete qgvItem;
        }
    }
    drawingLayer->mItems.clear();

    // Удаление всех меток из слоя `sloi`
    QList<QGraphicsItem*> sloiItems = sloi->mItems;
    for (QGraphicsItem* item : sloiItems)
    {
        QGVItem* qgvItem = dynamic_cast<QGVItem*>(item);
        if (qgvItem)
        {
            sloi->removeItem(qgvItem);
            delete qgvItem;
        }
    }
    sloi->mItems.clear();
}

