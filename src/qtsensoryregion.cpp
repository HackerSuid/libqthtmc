#include "qt.h"

#include <QGridLayout>

QtSensoryRegion::QtSensoryRegion(QWidget *parent, input_patterns *ip)
{
    this->parent = parent;
    this->patterns = ip;
    sensoryUnitGrid = NULL;
    motorUnitGrid = NULL;
    locUnitGrid = NULL;
}

QtSensoryRegion::~QtSensoryRegion()
{
}

QGridLayout* QtSensoryRegion::SensoryUnitGrid()
{
    if (sensoryUnitGrid)
        return sensoryUnitGrid;

    sensoryUnitGrid = new QGridLayout();
    sensoryUnitGrid->setSpacing(0);
    sensoryUnitGrid->setAlignment(Qt::AlignCenter);
    sdr_t bits = patterns->sensory_pattern;
    unsigned int w = patterns->sensory_sz.width;;
    unsigned int h = patterns->sensory_sz.height;

    for (unsigned int i=0; i<h; i++) {
        for (unsigned int j=0; j<w; j++) {
            QtUnit *unit = new QtUnit(
                bits[i][j],
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                0, 3, 3
            );
            if (bits[i][j])
                unit->setBrushColor(ACTIVE_COLOR);
            sensoryUnitGrid->addWidget(unit, i, j, 1, 1);
        }
    }

    return sensoryUnitGrid;
}

QGridLayout* QtSensoryRegion::LocationUnitGrid()
{
    if (locUnitGrid)
        return locUnitGrid;

    locUnitGrid = new QGridLayout();
    locUnitGrid->setSpacing(0);
    locUnitGrid->setAlignment(Qt::AlignCenter);
/*
    SensoryRegion *locPattern = pattern->GetLocationPattern();

    int w = locPattern->GetWidth();
    int h = locPattern->GetHeight();

    SensoryInput ***bits = locPattern->GetInput();

    // location signal is 1D, but partition it into segments for a
    // better visual.
    unsigned int n=0;
    for (int i=0; i<sqrt((double)w); i++) {
        for (int j=0; j<sqrt((double)w); j++, n++) {
            QtUnit *unit = new QtUnit(
                bits[0][n],
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                0, 6, 6
            );
            if (bits[0][n]->IsActive())
                unit->setBrushColor(ACTIVE_COLOR);
            locUnitGrid->addWidget(unit, i, j, 1, 1);
        }
    }
*/

    return locUnitGrid;
}

QGridLayout* QtSensoryRegion::MotorUnitGrid()
{
    return motorUnitGrid;
}
