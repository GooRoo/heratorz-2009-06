#include "orbit_gui.h"
#include "virtualmachine.h"

#include <algorithm>
#include <iostream>

enum SceneObjects
{
    EARTH,
    TARGET_RAD,
    SHIP,
    LAST
};

OrbitGui::OrbitGui()
    :mFuel(0), mX(0), mY(0), mScore(0), mTargetRad(0)
{
    mScene = new QGraphicsScene(QRectF(-300, -300, 600, 600));
    mSceneItems = new QGraphicsItem*[LAST];
    std::fill(mSceneItems, mSceneItems + LAST, reinterpret_cast<QGraphicsItem*>(NULL));
    
    QPen pen(Qt::DashLine); pen.setColor(Qt::yellow);
    mScene->setBackgroundBrush(QBrush(Qt::black));
    mScene->addLine(0, -300, 0, 300, pen);
    mScene->addLine(-300, 0, 300, 0, pen);
    
    mSceneItems[EARTH] = new OrbitEarth;
    mScene->addItem(mSceneItems[EARTH]);
    mSceneItems[EARTH]->setPos(0.0, 0.0);
    mSceneItems[EARTH]->setZValue(1);

    mSceneItems[SHIP] = new OrbitShip;
    mScene->addItem(mSceneItems[SHIP]);
    mSceneItems[SHIP]->setZValue(2);

    mView = new QGraphicsView(mScene);
    mView->resize(620, 620);
}


OrbitGui::~OrbitGui()
{
    delete mScene;
    delete mView;
    delete [] mSceneItems;
}

void OrbitGui::update()
{
	const double m = 2*10e+4;

    mX = mVM->readPort(0x2) / m;
    mY = mVM->readPort(0x3) / m;
    mTargetRad = mVM->readPort(0x4) / m;

    if (!mSceneItems[TARGET_RAD])
        mSceneItems[TARGET_RAD] = mScene->addEllipse(-mTargetRad, -mTargetRad , mTargetRad*2, mTargetRad*2, QPen(Qt::red), QBrush(Qt::NoBrush));

    mSceneItems[SHIP]->setPos(mX, mY);
    mScene->invalidate();
}

void OrbitGui::show()
{
    mView->show();
}

//////////////////////////////////////////////////////////////////////////


OrbitEarth::OrbitEarth()
{
    mPix = new QPixmap(":/images/Resources/earth.png");
}


OrbitEarth::~OrbitEarth()
{
    delete mPix;
}


void OrbitEarth::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(
            QPointF(-30, -30),
            QPixmap(mPix->scaled(QSize(60, 60), Qt::KeepAspectRatio, Qt::SmoothTransformation))
    );
    painter->setBrush(Qt::red);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QPointF(0, 0), 2, 2);
}

QRectF OrbitEarth::boundingRect() const
{
    return QRectF(mPix->scaled(QSize(60, 60), Qt::KeepAspectRatio, Qt::SmoothTransformation).rect());
}

//////////////////////////////////////////////////////////////////////////


OrbitShip::OrbitShip()
{
    mPix = new QPixmap(":/images/Resources/ship.png");
}


OrbitShip::~OrbitShip()
{
    delete mPix;
}


void OrbitShip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(
            QPointF(-25, -25),
            QPixmap(mPix->scaled(QSize(50, 50), Qt::KeepAspectRatio, Qt::SmoothTransformation))
        );
}

QRectF OrbitShip::boundingRect() const
{
    return QRectF(mPix->scaled(QSize(50, 50), Qt::KeepAspectRatio, Qt::SmoothTransformation).rect());
}

