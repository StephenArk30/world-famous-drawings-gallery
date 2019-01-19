/****************************************************************************
**
** Data Structure Project
** StephenArk30
**
****************************************************************************/

#ifndef GRAPHMODIFIER_H
#define GRAPHMODIFIER_H

#include <QtDataVisualization/q3dbars.h>
#include <QtDataVisualization/qbardataproxy.h>
#include <QtDataVisualization/qabstract3dseries.h>

#include <QtGui/QFont>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QPointer>
#include <QtCore/QPropertyAnimation>

using namespace QtDataVisualization;

class GraphModifier : public QObject
{
    Q_OBJECT
public:
    explicit GraphModifier(Q3DBars *bargraph);
    ~GraphModifier();

    void resetMapData(int row, int col); // 重设地图
    void changePresetCamera(); // 转换相机角度
    void changeFontSize(int fontsize); // 改变字号
    void rotateX(int rotation); // x轴旋转
    void rotateY(int rotation); // y轴旋转
    void setBackgroundEnabled(int enabled); // 显示背景
    void setGridEnabled(int enabled); // 显示网格
    void setMonitor(int row, int col, int **monitorMap); // 放置监视器（机器人）

public Q_SLOTS:
    void changeLabelRotation(int rotation);
    void setAxisTitleVisibility(bool enabled);
    void zoomToSelectedBar();

Q_SIGNALS:
    void backgroundEnabledChanged(bool enabled);
    void gridEnabledChanged(bool enabled);
    void fontSizeChanged(int size);

private:
    Q3DBars *m_graph; // 地图
    float m_xRotation; // x轴旋转量
    float m_yRotation; // y轴旋转量
    int m_fontSize; // 字体大小
    float m_minval; // z轴下界
    float m_maxval; // z轴上界
    QStringList x; // x轴坐标
    QStringList y; // y轴坐标
    // 坐标轴
    QValue3DAxis *m_zAxis;
    QCategory3DAxis *m_yAxis;
    QCategory3DAxis *m_xAxis;
    QBar3DSeries *m_Series; // 数据集
    QAbstract3DSeries::Mesh m_barMesh; // 形状
    // 摄像机
    QPropertyAnimation m_animationCameraX;
    QPropertyAnimation m_animationCameraY;
    QPropertyAnimation m_animationCameraZoom;
    QPropertyAnimation m_animationCameraTarget;
    float m_defaultAngleX;
    float m_defaultAngleY;
    float m_defaultZoom;
    QVector3D m_defaultTarget;
};

#endif
