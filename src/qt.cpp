#include "qt.h"
#include "htm.h"

#include <QtGui/QApplication>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtCore/QObject>
#include <QtGui/QLabel>
#include <QtGui/QAction>
#include <QtGui/QMenuBar>
#include <QtGui/QScrollArea>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QPainter>

QtFront::QtFront(struct layer *layer4)
{
    this->layer4 = layer4;
    activeWindow = NULL;

    DefaultLayout = new QVBoxLayout();
    DefaultLayout->setMargin(0);
    setStyleSheet(
        "QWidget#MainWidget {"
            "background-color: rgb(25, 25, 25);"
        "}"
        "QGroupBox {"
            "border: 2px solid gray;"
            "border-radius: 3px;"
            "margin-top: 0.5em;"
        "}"
        "QGroupBox::title {"
            "subcontrol-origin: margin;"
            "left: 10px;"
            "padding: 0px 3px 0px 3px;"
            "color: white;"
        "}"
        "QGroupBox#RegionId {"
            "background-color: rgb(25, 25, 25);"
        "}"
    );
}

QtFront::~QtFront()
{
}

void QtFront::LoadQt(void)
{
    // Set the configured window size.
    setFixedSize(WIN_WIDTH, WIN_HEIGHT);
    // Initialize the Qt display objects.
    CurrentInput = new QtSensoryRegion(this, get_htm_input_patterns());
    HtmDisplay = new QtHtm(this, layer4);
    // Initialize the widgets.
    CreateTrainingWidget();
    CreateActions();
    CreateMenuBar();
    // Initialize the layout.
    DefaultLayout->setMenuBar(menuBar);
    setLayout(DefaultLayout);
    // Display the default window widget.
    ShowTrainingWidget();
    // Display the UI from the application.
    this->show();
}

void QtFront::CreateActions()
{
    // Menu actions
    openAction = new QAction("Open", this);
    saveAction = new QAction("Save", this);
    exitAction = new QAction("Exit", this);

    trainAction = new QAction("Train", this);
    connect(trainAction, SIGNAL(triggered()),
            this, SLOT(ShowTrainingWidget()));

    // Window actions
    connect(TrainSingPattButton, SIGNAL(clicked()),
        this, SLOT(RunSinglePattern()));
    connect(TrainSingProgButton, SIGNAL(clicked()),
        this, SLOT(RunSingleProgram()));
    connect(TrainVarButton, SIGNAL(clicked()),
        this, SLOT(RunVariableProgram()));

    // Analysis actions (gnuplot)
    plotPredCompAction = new QAction("Plot prediction comprehension", this);
    connect(
        plotPredCompAction,
        SIGNAL(triggered()),
        this,
        SLOT(PlotPredCompGraph())
    );
}

void QtFront::CreateMenuBar()
{
    menuBar = new QMenuBar(this);

    fileMenu = menuBar->addMenu("File");
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(exitAction);

    execMenu = menuBar->addMenu("Execute");
    execMenu->addAction(trainAction);

    analysisMenu = menuBar->addMenu("Analysis");
    analysisMenu->addAction(plotPredCompAction);
}

void QtFront::CreateTrainingWidget()
{
    TrainingWindow = new QWidget();
    TrainingWindow->setFixedSize(this->width(), this->height());
    TrainingWindow->setObjectName("MainWidget");
    TrainingLayout = new QGridLayout();

    // groupbox for sensory input.
    sensoryGroup = new QGroupBox("proximal sensory input");
    sensoryGroup->setObjectName("RegionId");
    QGridLayout *sensoryGrid = CurrentInput->SensoryUnitGrid();
    sensoryGroup->setLayout(sensoryGrid);

    // groupbox for location input.
    locGroup = new QGroupBox("allocentric input");
    locGroup->setObjectName("RegionId");
    QGridLayout *locGrid = CurrentInput->LocationUnitGrid();
    locGroup->setLayout(locGrid);

    // Interactive controls
    QWidget *controls = new QWidget;
    controls->setStyleSheet("color: white;");
    QGridLayout *controlsLayout = new QGridLayout;
    controlsLayout->setHorizontalSpacing(1);
    controlsLayout->setVerticalSpacing(3);

    QLabel *singPattLab = new QLabel("Single pattern");
    TrainSingPattButton = new QPushButton("consume");
    TrainSingPattButton->setStyleSheet("color: black;");
    controlsLayout->addWidget(singPattLab, 0, 0);
    controlsLayout->addWidget(TrainSingPattButton, 0, 1);

    QLabel *singProgLab = new QLabel("Single program");
    TrainSingProgButton = new QPushButton("execute");
/*
    TrainSingProgButton->setStyleSheet("color: black;");
    controlsLayout->addWidget(singProgLab, 1, 0);
    controlsLayout->addWidget(TrainSingProgButton, 1, 1);
*/

    QLabel *VarLab = new QLabel("Variable program: ");
    VarEdit = new QLineEdit();
    VarEdit->setStyleSheet("color: black");
    TrainVarButton = new QPushButton("initiate");
    TrainVarButton->setStyleSheet("color: black;");
    controlsLayout->addWidget(VarLab, 1, 0);
    controlsLayout->addWidget(VarEdit, 1, 1);
    controlsLayout->addWidget(TrainVarButton, 2, 1);

    controlsLayout->setRowStretch(4, 4);
    controls->setLayout(controlsLayout);

    // groupbox for htm object details.
    objHtm = new QGroupBox("object details");
    objHtm->setStyleSheet("color: white;");

    // groupbox for htm region sublayer.
    smiLayerGroup = new QGroupBox("sublayer 0");
    smiLayerGroup->setObjectName("RegionId");
    QGridLayout *smiGrid = HtmDisplay->UnitGrid(objHtm);
    // connect first Htm region grid units to sensory input grid units.
    //HtmDisplay->SetQtSynapses(inputGrid);
    smiLayerGroup->setLayout(smiGrid);

    poolGroup = new QGroupBox("pooling layer");
    poolGroup->setObjectName("RegionId");
    QGridLayout *poolGrid = HtmDisplay->PoolUnitGrid(objHtm);
    poolGroup->setLayout(poolGrid);

    TrainingLayout->addWidget(sensoryGroup, 0, 0, 1, 1);
    TrainingLayout->addWidget(locGroup, 1, 0, 1, 1);
    //TrainingLayout->addWidget(objHtm, 1, 0, 1, 1);
    TrainingLayout->addWidget(controls, 2, 0, 1, 1);
    TrainingLayout->addWidget(smiLayerGroup, 0, 1, 3, 1);
    TrainingLayout->addWidget(poolGroup, 0, 2, 3, 1);

    TrainingLayout->setColumnStretch(0, 30);
    TrainingLayout->setColumnStretch(1, 100);
    TrainingLayout->setRowStretch(0, 2);
    TrainingLayout->setRowStretch(1, 8);
    TrainingLayout->setRowStretch(2, 8);
    //TrainingLayout->setRowStretch(3, 1);

    TrainingWindow->setLayout(TrainingLayout);
/*    QTabWidget *tab = new QTabWidget();
    QLabel *trainLab = new QLabel("Training tab");
    QLabel *testLab = new QLabel("Testing tab");
    tab->addTab(trainLab, "Train");
    tab->addTab(testLab, "Test");
    TrainingLayout->addWidget(tab, 0, 0);
*/

    TrainingWindow->setLayout(TrainingLayout);
}

void QtFront::ShowTrainingWidget()
{
    if (ActiveWindow()) {
        if (ActiveWindow() == TrainingWindow)
            return;
        else {
            DefaultLayout->removeWidget(ActiveWindow());
            ActiveWindow()->hide();
        }
    }
    SetActiveWindow(TrainingWindow);
    DefaultLayout->addWidget(TrainingWindow);
    if (!TrainingWindow->isVisible())
        TrainingWindow->show();
}

QWidget* QtFront::ActiveWindow()
{
    return this->activeWindow;
}

void QtFront::SetActiveWindow(QWidget *w)
{
    this->activeWindow = w;
}

void QtFront::UpdateQtDisplay()
{
    UpdateInputDisplay(get_htm_input_patterns());
    UpdateHtmDisplay();
/*
    predCompWindowVal->setText(
        QString::number(
            HtmDisplay->PredictionComprehensionMetric(),
            'e', 2
        )
    );
    predSpecWindowVal->setText(
        QString::number(
            HtmDisplay->PredictionSpecificityMetric(),
            'e', 2
        )
    );
*/
    this->repaint();
}

/*
 * Update the QtSensoryRegion grid display to reflect th
 current input
 * pattern by changing the brush color of the QtUnits.
 */
void QtFront::UpdateInputDisplay(input_patterns *patterns)
{
    QGridLayout *currSensoryGrid =
        (QGridLayout *)sensoryGroup->layout();
    QGridLayout *newSensoryGrid =
        CurrentInput->SensoryUnitGrid();
    //unsigned int w = patterns->sensory_sz.width;
    //unsigned int h = patterns->sensory_sz.height;

    //SensoryRegion *mp = newPattern->GetMotorPattern();
    //QGridLayout *currMotorGrid =
    //    (QGridLayout *)motorGroup->layout();
    //QGridLayout *newMotorGrid =
    //    CurrentInput->MotorUnitGrid();

    //SensoryRegion *lp = newPattern->GetLocationPattern();
    QGridLayout *currLocGrid =
        (QGridLayout *)locGroup->layout();
    QGridLayout *newLocGrid =
        CurrentInput->LocationUnitGrid();

    int r, g, b;
    QColor rgb;
    QtUnit *oldqtunit = NULL, *newqtunit = NULL;

    /* sensory input display grid */
    for (int i=0; i<newSensoryGrid->count(); i++) {
        int row, col, rs, cs;
        newSensoryGrid->getItemPosition(i, &row, &col, &rs, &cs);
        newqtunit = (QtUnit *)newSensoryGrid->itemAt(i)->widget();
        rgb = newqtunit->getBrushColor();
        rgb.getRgb(&r, &g, &b);
        oldqtunit = (QtUnit *)currSensoryGrid->itemAt(i)->widget();
        oldqtunit->setBrushColor(QColor(r, g, b));
    }
    /* motor input display grid */
    /*
    for (int i=0; i<newMotorGrid->count(); i++) {
        int row, col, rs, cs;
        newMotorGrid->getItemPosition(i, &row, &col, &rs
 &cs);
        newqtunit = (QtUnit *)newMotorGrid->itemAt(i)->w
dget();
        rgb = newqtunit->getBrushColor();
        rgb.getRgb(&r, &g, &b);
        oldqtunit = (QtUnit *)currMotorGrid->itemAt(i)->
idget();
        oldqtunit->setBrushColor(QColor(r, g, b));
        oldqtunit->SetActive(newqtunit->IsActive());
    }
    */

    /* location input display grid */
    for (unsigned int i=0; i<(unsigned int)newLocGrid->count(); i++) {
        int row, col, rs, cs;
        newLocGrid->getItemPosition(i, &row, &col, &rs, &cs);
        newqtunit = (QtUnit *)newLocGrid->itemAt(i)->widget();
        rgb = newqtunit->getBrushColor();
        rgb.getRgb(&r, &g, &b);
        oldqtunit = (QtUnit *)currLocGrid->itemAt(i)->widget();
        oldqtunit->setBrushColor(QColor(r, g, b));
        //oldqtunit->SetActive(newqtunit->IsActive());
    }
}

/*
 * Update the QtHtmSublayer grid display after running t
e CLA.
 */
void QtFront::UpdateHtmDisplay()
{
    QGridLayout *currGrid = (QGridLayout *)smiLayerGroup->layout();
    QGridLayout *newHtmGrid = HtmDisplay->UnitGrid(objHtm);
/*
    HtmSublayer **sublayers = htm->GetSublayers();
    unsigned int h = sublayers[0]->GetHeight();
    unsigned int w = sublayers[0]->GetWidth();
    unsigned int d = sublayers[0]->GetDepth();
*/
    unsigned int h = layer4->height;
    unsigned int w = layer4->width;
    QtUnit *colUnit = NULL;
/*
    QList<QtCell *> qtCells;
*/
    //QGridLayout *cellGrid = NULL;
/*
    QColor rgb[d+1];
    int r[d+1], g[d+1], b[d+1];
*/
    QColor rgb[1];
    int r[1], g[1], b[1];

    for (unsigned int i=0; i<h; i++) {
        for (unsigned int j=0; j<w; j++) {
            colUnit = (QtUnit *)newHtmGrid->itemAtPosition(i, j)->widget();
            //cellGrid = colUnit->GetCellGrid();
            rgb[0] = colUnit->getBrushColor();
            rgb[0].getRgb(&r[0], &g[0], &b[0]);
/*
            qtCells = colUnit->findChildren<QtCell *>();
            for (unsigned int c=1; c<=d; c++) {
                rgb[c] = qtCells.at(c-1)->getBrushColor();
                rgb[c].getRgb(&r[c], &g[c], &b[c]);
            }
*/
            colUnit =
                (QtUnit *)currGrid->itemAtPosition(i, j)->widget();
            colUnit->setBrushColor(QColor(r[0], g[0], b[0]));
/*
            qtCells = colUnit->findChildren<QtCell *>();
            for (unsigned int c=1; c<=d; c++)
                qtCells.at(c-1)->setBrushColor(
                    QColor(r[c], g[c], b[c])
                );
*/
        }
    }

/*
    QGridLayout *currPoolingGrid = (QGridLayout *)poolGroup->layout();
    QGridLayout *newPoolingGrid = HtmDisplay->PoolUnitGrid(objHtm);
    PoolingLayer *p = htm->GetPoolingLayer();
    h = p->GetHeight();
    w = p->GetWidth();
    QColor rgbPool;
    int rP, gP, bP;

    for (unsigned int i=0; i<h; i++) {
        for (unsigned int j=0; j<w; j++) {
            colUnit =
                (QtUnit *)newPoolingGrid->itemAtPosition(i, j)->widget();
            rgbPool = colUnit->getBrushColor();
            rgbPool.getRgb(&rP, &gP, &bP);
            colUnit =
                (QtUnit *)currPoolingGrid->itemAtPosition(i, j)->widget();
            colUnit->setBrushColor(
                QColor(rP, gP, bP));
        }
    }
*/
}

// BEGIN QT SLOTS

// Consume the next pattern of the program.
void QtFront::RunSinglePattern()
{
    if (!Run())
        return;

    // update the various Qt displays
    UpdateQtDisplay();
}

// Execute the program once and consume all the patterns
void QtFront::RunSingleProgram()
{
    while (Run())
        ;

    UpdateQtDisplay();
}

// Initiate a series of program executions.
void QtFront::RunVariableProgram()
{
    int n = VarEdit->text().toInt();
    for (int i=0; i<n; i++) {
        while (Run())
            ;
    }

    UpdateQtDisplay();
}

int QtFront::Run()
{
    input_patterns *patterns=NULL;

    process_subcortical_input();

    patterns = get_htm_input_patterns();
    if (patterns == NULL) {
/*
        htm->ReloadCodecTarget();
        htm->ConnectSubcorticalInput(true);
        return 0;
*/
    }

    CurrentInput = new QtSensoryRegion(this, patterns);
    HtmDisplay = new QtHtm(this, layer4);

    return 1;
}

void QtFront::PlotPredCompGraph()
{
    //htm->GeneratePredCompX11Gnuplot();
}

// END QT SLOTS

// paintEvent() event handler is called when:
// 1) the widget window is first shown
// 2) call to QWidget::update() or QWidget::repaint()
//    - update() is blocked, repaint() is not.
void QtFront::paintEvent(QPaintEvent *e)
{
    //this->UpdateDisplay();
}

void QtFront::closeEvent(QCloseEvent *e)
{
}

