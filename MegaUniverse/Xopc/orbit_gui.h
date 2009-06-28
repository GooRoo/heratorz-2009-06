#ifndef _ORBIT_GUI_H_
#define _ORBIT_GUI_H_

#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include "abstract_gui.h"
#include "model.h"

class VirtualMachine;

class OrbitGui : public AbstractGui
{
public:
	OrbitGui();
	virtual ~OrbitGui();

	void update();
    void show();

private:
	double mX, mY, mScore, mFuel, mTargetRad;
    QGraphicsView * mView;
    QGraphicsScene * mScene;
    QGraphicsItem ** mSceneItems;

    PhysxModel mModel;
};

//////////////////////////////////////////////////////////////////////////

class OrbitEarth : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    OrbitEarth();
    virtual ~OrbitEarth();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    QPixmap * mPix;
};

//////////////////////////////////////////////////////////////////////////

class OrbitShip: public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    OrbitShip();
    virtual ~OrbitShip();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    QPixmap * mPix;
};



#endif // _ORBIT_GUI_H_

