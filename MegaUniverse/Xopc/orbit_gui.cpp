#include "orbit_gui.h"
#include "virtualmachine.h"

#include <algorithm>
#include <iostream>
#include <sstream>

enum SceneObjects
{
    EARTH,
    TARGET_RAD,
    SHIP,
    TEXT,
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

    mSceneItems[TEXT] = NULL;

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
	const double m = 2e+5;

    mScore = mVM->readPort(0x0);
    mFuel = mVM->readPort(0x1);
    mX = mVM->readPort(0x2);
    mY = mVM->readPort(0x3);
    mTargetRad = mVM->readPort(0x4);

    if (!mSceneItems[TARGET_RAD])
        mSceneItems[TARGET_RAD] = mScene->addEllipse(
                -mTargetRad / m,
                -mTargetRad / m,
                mTargetRad * 2 / m,
                mTargetRad * 2 / m,
                QPen(Qt::red),
                QBrush(Qt::NoBrush)
            );

    mSceneItems[SHIP]->setPos(mX / m, mY / m);
	

    static QGraphicsTextItem * text = new QGraphicsTextItem("");
    if (mSceneItems[TEXT] != NULL)
    {
        mScene->removeItem(mSceneItems[TEXT]);
    }
    std::stringstream outText;
    outText << "<font color=\"yellow\"><strong>Coords:</strong> (" << mX << ", " << mY << ")<br/>\n" <<
        "<strong>Fuel:</strong> " << mFuel <<
        "<br/>\n<strong>Cur.radius:</strong> " << sqrt(mX * mX + mY * mY) <<
        "<br/>\n<strong>Target rad.:</strong> " << mTargetRad <<
        "<br/>\n<strong>Score:</strong> " << mScore << "</font>";
    text->setHtml(outText.str().c_str());
    mSceneItems[TEXT] = text;
    mScene->addItem(mSceneItems[TEXT]);
    text->setPos(20 - 300, 20 - 300);
    
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
            QPointF(-20, -20),
            QPixmap(mPix->scaled(QSize(40, 40), Qt::KeepAspectRatio, Qt::SmoothTransformation))
        );
    painter->setBrush(Qt::green);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QPointF(0, 0), 2, 2);
}

QRectF OrbitShip::boundingRect() const
{
    return QRectF(mPix->scaled(QSize(40, 40), Qt::KeepAspectRatio, Qt::SmoothTransformation).rect());
}

