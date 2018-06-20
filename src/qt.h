#ifndef QT_H_
#define QT_H_ 1

#include <QObject> // slots/signals
#include <QWidget> // qwidget inheritance
#include <QApplication>

#include "qt.h"
#include "htm.h"

#define WIN_WIDTH 1880
#define WIN_HEIGHT 1024

#define DISPLAY_X   10
#define DISPLAY_Y   100
#define CELL_DIA    8.0
#define COL_DIA     ((CELL_DIA*2.5)+CELL_DIA)

#define DEF_UNIT_W  16
#define DEF_UNIT_H  16

#define DEF_CELL_W  5
#define DEF_CELL_H  5

/* forward decls for qthtmc */
class QtHtm;
class QtSensoryRegion;
class SensoryRegion;
/* forward decls for qt widgets */
class QVBoxLayout;
class QWidget;
class QGridLayout;
class QGroupBox;
class QLabel;
class QPushButton;
class QLineEdit;
class QWidget;
class QMenuBar;
class QMenu;
class QAction;

/* exported htm config */
extern struct htm_conf *htmconf;

/* dark white/very light grey */
const QColor ACTIVE_COLOR = QColor(180, 180, 180);
/* medium grey */
const QColor INACTIVE_COLOR = QColor(90, 90, 90);
/* bright yellow */
const QColor PREDICTED_COLOR = QColor(255, 255, 102);
/* some sort of orange/red */
const QColor HIGHLIGHT_INACTIVE = QColor(0xFF, 0xAA, 0x3
);
/* green */
const QColor HIGHLIGHT_ACTIVE = QColor(0x3C, 0xC9, 0x34);
/* kinda light blue */
const QColor HOVER_COLOR = QColor(0x55, 0x79, 0xFF);

class QtFront : public QWidget
{
Q_OBJECT
public:
    QtFront(struct layer *layer4);
    ~QtFront();
    void LoadQt(void);
    void UpdateQtDisplay();
    void UpdateInputDisplay(input_patterns *patterns);
    void UpdateHtmDisplay();
    QtSensoryRegion* GetCurrentInputDisplay() { return CurrentInput; }
    void CreateActions();
    void CreateMenuBar();
    void CreateTrainingWidget();
    void SetActiveWindow(QWidget *w);
    QWidget* ActiveWindow();
private:
    /* the Qt QApplication object for controlling the GUI */
    QApplication *app;
    struct layer *layer4;

    /* Window organization members */
    QVBoxLayout *DefaultLayout;
    /* training window widgets */
    QWidget *TrainingWindow;
    QGridLayout *TrainingLayout;
    QGroupBox *sensoryGroup, *motorGroup, *locGroup;
    QGroupBox *smiLayerGroup, *poolGroup, *objHtm;
    QLabel *predCompWindowVal, *predSpecWindowVal;
    QPushButton *TrainSingPattButton, *TrainSingProgButton, *TrainVarButton;
    QLineEdit *VarEdit;

    QWidget *activeWindow;

    QMenuBar *menuBar;
    QMenu *fileMenu, *configMenu, *execMenu, *analysisMenu;

    QAction *openAction, *saveAction, *exitAction;
    QAction *trainAction;
    QAction *plotPredCompAction;

    // Qt classes to display Htm features
    QtSensoryRegion *CurrentInput;
    QtHtm *HtmDisplay;
protected:
    // override virtual methods from QWidget
    void paintEvent(QPaintEvent *e);
    void closeEvent(QCloseEvent *e);
private slots:
    void ShowTrainingWidget();
    void RunSinglePattern();
    void RunSingleProgram();
    void RunVariableProgram();
    int Run();
    void PlotPredCompGraph();
};

class QtSensoryRegion : public QWidget
{
Q_OBJECT
private:
    QWidget *parent;
    input_patterns *patterns;
    QGridLayout *sensoryUnitGrid, *motorUnitGrid, *locUnitGrid;
public:
    QtSensoryRegion(QWidget *parent, input_patterns *ip);
    ~QtSensoryRegion();
    QGridLayout* SensoryUnitGrid();
    QGridLayout* LocationUnitGrid();
    QGridLayout* MotorUnitGrid();
    SensoryRegion* GetPattern();
};

class QtHtm : public QWidget
{
Q_OBJECT
private:
    QWidget *parent;
    struct layer *layer4;
    QGridLayout *htmUnitGrid, *poolUnitGrid;

/*
    float SlidingWindowAvg(std::list<float> window);
*/
public:
    QtHtm(QWidget *parent, struct layer *layer4);
    ~QtHtm();
    QGridLayout* UnitGrid(QGroupBox *objHtm);
    QGridLayout* PoolUnitGrid(QGroupBox *objHtm);
/*
    float PredictionComprehensionMetric();
    float PredictionSpecificityMetric();
    void SetQtSynapses(QGridLayout *inputGrid);
    void DrawCell(QPainter *p, unsigned int x, unsigned int y);
*/
};

class QtUnit : public QWidget
{
Q_OBJECT
public:
    QtUnit(
        char in_bit,
        QGroupBox *objHtm,
        QGridLayout *htmGrid,
        QGridLayout *sensoryGrid,
        QGridLayout *motorGrid,
        QGridLayout *locGrid,
        QGridLayout *proxInputGrid,
        int c, int w=DEF_UNIT_W, int h=DEF_UNIT_H
    );
    ~QtUnit();
    void CreateActions();
    QSize sizeHint() const;
    void setBrushColor(const QColor &newColor);
    QColor getBrushColor() const;
    void SaveBrushColor() { prevBrushColor = brushColor; }
    void RestoreBrushColor() { brushColor = prevBrushColor; }
    QGridLayout* GetCellGrid();

    void SetClickable(bool flag);
    bool IsClickable();
protected:
    void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
private:
    char in_bit;
    QGridLayout *htmGrid, *sensoryGrid, *motorGrid;
    QGridLayout *locGrid, *proxInputGrid;
    // context menu members
    QAction *showProximalConnections, *hideProximalConnections;
    // cell layout members
    QGridLayout *CellGrid;
    QColor brushColor, prevBrushColor;
    int sizeW, sizeH;
    int cells;;
    QGroupBox *objDetail;
    // other members
    bool isClickable;
    bool toggled;

    void _ToggleProximalConnections(bool flag);
private slots:
    void ShowProximalConnections();
    void HideProximalConnections();
};

#endif

