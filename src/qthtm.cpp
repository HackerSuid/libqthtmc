#include "qt.h"

#include "htm.h"

#include <QGridLayout>
#include <stdio.h>

QtHtm::QtHtm(QWidget *parent, struct layer *layer4)
    : QWidget(parent)
{
    this->layer4 = layer4;
    htmUnitGrid = NULL;
    poolUnitGrid = NULL;
}

QtHtm::~QtHtm()
{
}

QGridLayout* QtHtm::UnitGrid(QGroupBox *objHtm)
{
    if (htmUnitGrid)
        return htmUnitGrid;

    htmUnitGrid = new QGridLayout();
    htmUnitGrid->setSpacing(0);
    htmUnitGrid->setAlignment(Qt::AlignCenter);

    unsigned int w = layer4->width;
    unsigned int h = layer4->height;
    struct minicolumn ***mc = layer4->minicolumns;

    QtSensoryRegion *qtinput =
        ((QtFront *)parentWidget())->GetCurrentInputDisplay();
    QGridLayout *sensoryGrid = qtinput->SensoryUnitGrid();
    QGridLayout *motorGrid = qtinput->MotorUnitGrid();
    QGridLayout *locGrid = qtinput->LocationUnitGrid();
    for (unsigned int i=0; i<h; i++) {
        for (unsigned int j=0; j<w; j++) {
            QtUnit *unit = new QtUnit(
                mc_active_at(mc[i][j], 0),
                objHtm,
                htmUnitGrid,
                sensoryGrid,
                motorGrid,
                locGrid,
                sensoryGrid,
                0
            );
            unit->SetClickable(true);
            if (mc_active_at(mc[i][j], 0))
                unit->setBrushColor(ACTIVE_COLOR);
            htmUnitGrid->addWidget(unit, i, j, 1, 1);
        }
    }

    return htmUnitGrid;
}

QGridLayout* QtHtm::PoolUnitGrid(QGroupBox *objHtm)
{
    if (poolUnitGrid)
        return poolUnitGrid;

/*
    PoolingLayer *pL = htm->GetPoolingLayer();
    int h = pL->GetHeight();
    int w = pL->GetWidth();
    Column ***cols = (Column ***)pL->GetInput();
*/

    poolUnitGrid = new QGridLayout();
    poolUnitGrid->setSpacing(0);
    poolUnitGrid->setAlignment(Qt::AlignCenter);

    unsigned int h = 24;
    unsigned int w = 24;

    QtSensoryRegion *qtinput =
        ((QtFront *)parentWidget())->GetCurrentInputDisplay();
    QGridLayout *sensoryGrid = qtinput->SensoryUnitGrid();
    QGridLayout *motorGrid = qtinput->MotorUnitGrid();
    QGridLayout *locGrid = qtinput->LocationUnitGrid();

    for (unsigned int i=0; i<h; i++) {
        for (unsigned int j=0; j<w; j++) {
            QtUnit *unit = new QtUnit(
                0,
                objHtm,
                htmUnitGrid,
                sensoryGrid,
                motorGrid,
                locGrid,
                htmUnitGrid,
                0
            );
            unit->SetClickable(true);
            if (0)
                unit->setBrushColor(ACTIVE_COLOR);
            poolUnitGrid->addWidget(unit, i, j, 1, 1);
        }
    }

    return poolUnitGrid;
}
