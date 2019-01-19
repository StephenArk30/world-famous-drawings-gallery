#include "graphmodifier.h"
#include "solution.h"

#include <QtDataVisualization/qcategory3daxis.h>
#include <QtDataVisualization/qvalue3daxis.h>
#include <QtDataVisualization/qbardataproxy.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/qbar3dseries.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtCore/QTime>
#include <QtWidgets/QComboBox>
#include <QtCore/qmath.h>

using namespace QtDataVisualization;

//! [0]
// 初始化
GraphModifier::GraphModifier(Q3DBars *bargraph)
    : m_graph(bargraph),
      m_xRotation(0.0f), // 摄像机旋转
      m_yRotation(0.0f),
      m_fontSize(30),
      m_minval(0),
      m_maxval(2),
      //! [1]
      m_zAxis(new QValue3DAxis),
      m_yAxis(new QCategory3DAxis),
      m_xAxis(new QCategory3DAxis),
      m_Series(new QBar3DSeries),
      //! [1]
      m_barMesh(QAbstract3DSeries::MeshBevelBar) // 形状是立方体
{
    //! [2]
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftMedium);
    QColor color(255, 140, 0); // 地图颜色
    m_graph->activeTheme()->setBackgroundColor(color);
    m_graph->activeTheme()->setBackgroundEnabled(true);
    m_graph->activeTheme()->setFont(QFont("Times New Roman", m_fontSize));
    m_graph->activeTheme()->setLabelBackgroundEnabled(true);
    color.setRgb(255, 255, 255);
    m_graph->activeTheme()->setGridLineColor(color); // 网格颜色
    color.setRgb(0, 0, 0);
    QList<QColor> colors;
    colors.push_front(Qt::black);
    m_graph->activeTheme()->setBaseColors(colors); // 立方体颜色
    m_graph->activeTheme()->setSingleHighlightColor(Qt::white); // 选中时的立方体颜色
    //! [2]

    //! [3]
    m_zAxis->setTitle("Gallery"); // z轴标签
    m_zAxis->setSegmentCount(1); // 主分段
    m_zAxis->setSubSegmentCount(1); // 副分段
    m_zAxis->setRange(m_minval, m_maxval); // z轴范围
    m_zAxis->setLabelFormat(QString(QStringLiteral(""))); // z轴分度值（空）
    m_zAxis->setLabelAutoRotation(30.0f); // 标签旋转
    m_zAxis->setTitleVisible(true);

    // x, y轴标签
    m_yAxis->setTitle("y");
    m_yAxis->setLabelAutoRotation(30.0f);
    m_yAxis->setTitleVisible(true);
    m_xAxis->setTitle("x");
    m_xAxis->setLabelAutoRotation(30.0f);
    m_xAxis->setTitleVisible(true);

    // 设置3D Bars各轴
    m_graph->setValueAxis(m_zAxis);
    m_graph->setRowAxis(m_yAxis);
    m_graph->setColumnAxis(m_xAxis);
    //! [3]

    //! [8]
    // 选中后显示标签
    m_Series->setItemLabelFormat(QStringLiteral("(@colLabel ,@rowLabel)"));
    m_Series->setMesh(m_barMesh); // 设置形状
    m_Series->setMeshSmooth(true); // 设置光滑
    //! [8]

    //! [4]
    m_graph->addSeries(m_Series);
    //! [4]

    //! [6]
    changePresetCamera();
    //! [6]

    //! [9]
    resetMapData(5, 5); // 初始地图5X5
    //! [9]

    // Set up property animations for zooming to the selected bar
    //! [12]
    Q3DCamera *camera = m_graph->scene()->activeCamera();
    m_defaultAngleX = camera->xRotation();
    m_defaultAngleY = camera->yRotation();
    m_defaultZoom = camera->zoomLevel();
    m_defaultTarget = camera->target();

    m_animationCameraX.setTargetObject(camera);
    m_animationCameraY.setTargetObject(camera);
    m_animationCameraZoom.setTargetObject(camera);
    m_animationCameraTarget.setTargetObject(camera);

    m_animationCameraX.setPropertyName("xRotation");
    m_animationCameraY.setPropertyName("yRotation");
    m_animationCameraZoom.setPropertyName("zoomLevel");
    m_animationCameraTarget.setPropertyName("target");

    int duration = 1700;
    m_animationCameraX.setDuration(duration);
    m_animationCameraY.setDuration(duration);
    m_animationCameraZoom.setDuration(duration);
    m_animationCameraTarget.setDuration(duration);

    // The zoom always first zooms out above the graph and then zooms in
    qreal zoomOutFraction = 0.3;
    m_animationCameraX.setKeyValueAt(zoomOutFraction, QVariant::fromValue(0.0f));
    m_animationCameraY.setKeyValueAt(zoomOutFraction, QVariant::fromValue(90.0f));
    m_animationCameraZoom.setKeyValueAt(zoomOutFraction, QVariant::fromValue(50.0f));
    m_animationCameraTarget.setKeyValueAt(zoomOutFraction,
                                          QVariant::fromValue(QVector3D(0.0f, 0.0f, 0.0f)));
    //! [12]
}
//! [0]

GraphModifier::~GraphModifier()
{
    delete m_graph;
}

// 重新设置机器人地图
void GraphModifier::resetMapData(int row, int col)
{
    //! [5]
    // Set up data
    int **monitorMap;

    x.clear();
    y.clear();

    // Create data arrays
    QBarDataArray *dataSet = new QBarDataArray;
    QBarDataRow *dataRow;

    monitorMap = new int*[row];
    float val = 2.5 / (row > col ? row : col); // 这个val是用来使立方体保持立方的
    int i, j;
    for(i = 0; i < row; i++) monitorMap[i] = new int[col];
    setMonitor(row, col, monitorMap); // 通过算法设置地图

    // 设置x, y轴标签分度值（1，2，3...）
    for(i = 0; i < row; i++)
        y << QString::number(i);
    for(i = 0; i < col; i++)
        x << QString::number(i);

    dataSet->reserve(row); // 设置data array大小
    for (i = 0; i < row; i++) {
        // Create a data row
        dataRow = new QBarDataRow(col);
        for (j = 0; j < col; j++) {
            // Add data to the row
            (*dataRow)[j].setValue(monitorMap[i][j] * val);
        }
        // Add the row to the set
        dataSet->append(dataRow);
    }

    // Add data to the data proxy (the data proxy assumes ownership of it)
    m_Series->dataProxy()->resetArray(dataSet, y, x);
    //! [5]
}

// 更改相机视角
void GraphModifier::changePresetCamera()
{
    m_animationCameraX.stop();
    m_animationCameraY.stop();
    m_animationCameraZoom.stop();
    m_animationCameraTarget.stop();

    // Restore camera target in case animation has changed it
    m_graph->scene()->activeCamera()->setTarget(QVector3D(0.0f, 0.0f, 0.0f));

    //! [10]
    static int preset = Q3DCamera::CameraPresetFront;

    m_graph->scene()->activeCamera()->setCameraPreset((Q3DCamera::CameraPreset)preset);

    if (++preset > Q3DCamera::CameraPresetDirectlyBelow)
        preset = Q3DCamera::CameraPresetFrontLow;
    //! [10]
}

// 修改字号
void GraphModifier::changeFontSize(int fontsize)
{
    m_fontSize = fontsize;
    QFont font = m_graph->activeTheme()->font();
    font.setPointSize(m_fontSize);
    m_graph->activeTheme()->setFont(font);
}

// 设置轴标签旋转
void GraphModifier::changeLabelRotation(int rotation)
{
    m_zAxis->setLabelAutoRotation(float(rotation));
    m_xAxis->setLabelAutoRotation(float(rotation));
    m_yAxis->setLabelAutoRotation(float(rotation));
}

// 设置轴名隐藏
void GraphModifier::setAxisTitleVisibility(bool enabled)
{
    m_zAxis->setTitleVisible(enabled);
    m_xAxis->setTitleVisible(enabled);
    m_yAxis->setTitleVisible(enabled);
}

//! [11]
// 设置相机缩放
void GraphModifier::zoomToSelectedBar()
{
    m_animationCameraX.stop();
    m_animationCameraY.stop();
    m_animationCameraZoom.stop();
    m_animationCameraTarget.stop();

    Q3DCamera *camera = m_graph->scene()->activeCamera();
    float currentX = camera->xRotation();
    float currentY = camera->yRotation();
    float currentZoom = camera->zoomLevel();
    QVector3D currentTarget = camera->target();

    m_animationCameraX.setStartValue(QVariant::fromValue(currentX));
    m_animationCameraY.setStartValue(QVariant::fromValue(currentY));
    m_animationCameraZoom.setStartValue(QVariant::fromValue(currentZoom));
    m_animationCameraTarget.setStartValue(QVariant::fromValue(currentTarget));

    QPoint selectedBar = m_graph->selectedSeries()
            ? m_graph->selectedSeries()->selectedBar()
            : QBar3DSeries::invalidSelectionPosition();

    if (selectedBar != QBar3DSeries::invalidSelectionPosition()) {
        // Normalize selected bar position within axis range to determine target coordinates
        //! [13]
        QVector3D endTarget;
        float xMin = m_graph->columnAxis()->min();
        float xRange = m_graph->columnAxis()->max() - xMin;
        float zMin = m_graph->rowAxis()->min();
        float zRange = m_graph->rowAxis()->max() - zMin;
        endTarget.setX((selectedBar.y() - xMin) / xRange * 2.0f - 1.0f);
        endTarget.setZ((selectedBar.x() - zMin) / zRange * 2.0f - 1.0f);
        //! [13]

        // Rotate the camera so that it always points approximately to the graph center
        //! [15]
        qreal endAngleX = qAtan(qreal(endTarget.z() / endTarget.x())) / M_PI * -180.0 + 90.0;
        if (endTarget.x() > 0.0f)
            endAngleX -= 180.0f;
        float barValue = m_graph->selectedSeries()->dataProxy()->itemAt(selectedBar.x(),
                                                                        selectedBar.y())->value();
        float endAngleY = barValue >= 0.0f ? 30.0f : -30.0f;
        if (m_graph->valueAxis()->reversed())
            endAngleY *= -1.0f;
        //! [15]

        m_animationCameraX.setEndValue(QVariant::fromValue(float(endAngleX)));
        m_animationCameraY.setEndValue(QVariant::fromValue(endAngleY));
        m_animationCameraZoom.setEndValue(QVariant::fromValue(250));
        //! [14]
        m_animationCameraTarget.setEndValue(QVariant::fromValue(endTarget));
        //! [14]
    } else {
        // No selected bar, so return to the default view
        m_animationCameraX.setEndValue(QVariant::fromValue(m_defaultAngleX));
        m_animationCameraY.setEndValue(QVariant::fromValue(m_defaultAngleY));
        m_animationCameraZoom.setEndValue(QVariant::fromValue(m_defaultZoom));
        m_animationCameraTarget.setEndValue(QVariant::fromValue(m_defaultTarget));
    }

    m_animationCameraX.start();
    m_animationCameraY.start();
    m_animationCameraZoom.start();
    m_animationCameraTarget.start();
}
//! [11]

//! [7]
// 绕x旋转
void GraphModifier::rotateX(int rotation)
{
    m_xRotation = rotation;
    m_graph->scene()->activeCamera()->setCameraPosition(m_xRotation, m_yRotation);
}

// 绕y旋转
void GraphModifier::rotateY(int rotation)
{
    m_yRotation = rotation;
    m_graph->scene()->activeCamera()->setCameraPosition(m_xRotation, m_yRotation);
}
//! [7]

// 设置背景可见
void GraphModifier::setBackgroundEnabled(int enabled)
{
    m_graph->activeTheme()->setBackgroundEnabled(bool(enabled));
}

// 设置网格可见
void GraphModifier::setGridEnabled(int enabled)
{
    m_graph->activeTheme()->setGridEnabled(bool(enabled));
}

// 放置监视器
void GraphModifier::setMonitor(int row, int col, int **monitorMap)
{
    // row, col: 陈列馆大小 row * col
    // 陈列馆地图 monitorMap，放了机器人的 monitorMap[i][j] = 1，没放的为0。
    Solution solution(row, col);
    solution.SetMap(monitorMap);
}
