/****************************************************************************
**
** Data Structure Project
** StephenArk30
**
****************************************************************************/

#include "graphmodifier.h"
#include "inputform.h"

#include <iostream>
#include <QTextBrowser>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QScreen>
#include <QtGui/QFontDatabase>
#include <QLineEdit>

int main(int argc, char **argv)
{
    //! [0]
    // initialization
    QApplication app(argc, argv);
    Q3DBars *widgetgraph = new Q3DBars();
    QWidget *container = QWidget::createWindowContainer(widgetgraph);
    //! [0]

    // if open failed
    if (!widgetgraph->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return -1;
    }

    // set size
    QSize screenSize = widgetgraph->screen()->size();
    // set width and height
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    //! [1]
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget); // horizonal layout
    QVBoxLayout *vLayout = new QVBoxLayout(); // vertical layout
    hLayout->addWidget(container, 1); // a 3d image on the left
    hLayout->addLayout(vLayout); // a set of widgets on the right
    //! [1]

    // window title
    widget->setWindowTitle(QStringLiteral("WORLD-FAMOUS DRAWINGS GALLERY"));

    QPushButton *cameraButton = new QPushButton(widget);
    cameraButton->setText(QStringLiteral("Change camera preset"));

    QPushButton *zoomToSelectedButton = new QPushButton(widget);
    zoomToSelectedButton->setText(QStringLiteral("Zoom to selected bar"));

    QCheckBox *backgroundCheckBox = new QCheckBox(widget);
    backgroundCheckBox->setText(QStringLiteral("Show background"));
    backgroundCheckBox->setChecked(true);

    QCheckBox *gridCheckBox = new QCheckBox(widget);
    gridCheckBox->setText(QStringLiteral("Show grid"));
    gridCheckBox->setChecked(true);

    //! [4]
    // rotate
    QSlider *rotationSliderX = new QSlider(Qt::Horizontal, widget);
    rotationSliderX->setTickInterval(30);
    rotationSliderX->setTickPosition(QSlider::TicksBelow);
    rotationSliderX->setMinimum(-180);
    rotationSliderX->setValue(0);
    rotationSliderX->setMaximum(180);
    QSlider *rotationSliderY = new QSlider(Qt::Horizontal, widget);
    rotationSliderY->setTickInterval(15);
    rotationSliderY->setTickPosition(QSlider::TicksAbove);
    rotationSliderY->setMinimum(-90);
    rotationSliderY->setValue(0);
    rotationSliderY->setMaximum(90);
    //! [4]

    // font size
    QSlider *fontSizeSlider = new QSlider(Qt::Horizontal, widget);
    fontSizeSlider->setTickInterval(10);
    fontSizeSlider->setTickPosition(QSlider::TicksBelow);
    fontSizeSlider->setMinimum(1);
    fontSizeSlider->setValue(30);
    fontSizeSlider->setMaximum(100);

    QCheckBox *axisTitlesVisibleCB = new QCheckBox(widget);
    axisTitlesVisibleCB->setText(QStringLiteral("Axis titles visible"));
    axisTitlesVisibleCB->setChecked(true);

    QSlider *axisLabelRotationSlider = new QSlider(Qt::Horizontal, widget);
    axisLabelRotationSlider->setTickInterval(10);
    axisLabelRotationSlider->setTickPosition(QSlider::TicksBelow);
    axisLabelRotationSlider->setMinimum(0);
    axisLabelRotationSlider->setValue(30);
    axisLabelRotationSlider->setMaximum(90);

    // declaration of the question
    QString description = "世界名画陈列馆由m×n个排列成矩形阵列的陈列室组成。";
    description.append("为了防止名画被盗，需要在陈列室中设置警卫机器人哨位。");
    description.append("每个警卫机器人除了监视它所在的陈列室外，");
    description.append("还可以监视与它所在的陈列室相邻的上、下、左、右");
    description.append("4个陈列室。");
    description.append("试设计一个安排警卫机器人哨位的算法，");
    description.append("使得名画陈列馆中每一个陈列室都在警卫机器人的监视之下，");
    description.append("且所用的警卫机器人数最少。");
    QTextEdit *decp = new QTextEdit(widget);
    decp->setText(description);
    decp->setReadOnly(true);
    decp->setStyleSheet("background-color:lightYellow;");

    // row and column input
    InputForm *inputform = new InputForm(widget);
    QPushButton *rcinputButton = new QPushButton(widget);
    rcinputButton->setText(QStringLiteral("Reset map"));

    vLayout->addWidget(new QLabel(QStringLiteral("Question description")));
    vLayout->addWidget(decp);
    vLayout->addWidget(new QLabel(QStringLiteral("Input Row and Column(1 to 15)")));
    vLayout->addWidget(inputform->rowInput);
    vLayout->addWidget(inputform->colInput);
    vLayout->addWidget(rcinputButton);

    //! [5]
    // rotation form
    vLayout->addWidget(new QLabel(QStringLiteral("Rotate horizontally")));
    vLayout->addWidget(rotationSliderX, 0, Qt::AlignTop);
    vLayout->addWidget(new QLabel(QStringLiteral("Rotate vertically")));
    vLayout->addWidget(rotationSliderY, 0, Qt::AlignTop);
    //! [5]
    vLayout->addWidget(cameraButton, 0, Qt::AlignTop);
    vLayout->addWidget(zoomToSelectedButton, 0, Qt::AlignTop);
    vLayout->addWidget(backgroundCheckBox);
    vLayout->addWidget(gridCheckBox);
    vLayout->addWidget(axisTitlesVisibleCB);
    vLayout->addWidget(new QLabel(QStringLiteral("Adjust font size")));
    vLayout->addWidget(fontSizeSlider);
    vLayout->addWidget(new QLabel(QStringLiteral("Axis label rotation")));
    vLayout->addWidget(axisLabelRotationSlider, 1, Qt::AlignTop);
    //! [2]
    GraphModifier *modifier = new GraphModifier(widgetgraph);
    //! [2]

    // signals
    //! [6]
    // rotate signals
    QObject::connect(rotationSliderX, &QSlider::valueChanged, modifier, &GraphModifier::rotateX);
    QObject::connect(rotationSliderY, &QSlider::valueChanged, modifier, &GraphModifier::rotateY);
    //! [6]

    // reset map signals
    QObject::connect(rcinputButton, &QPushButton::clicked, inputform, &InputForm::setMapSize);
    QObject::connect(inputform, &InputForm::sentMapSize, modifier, &GraphModifier::resetMapData);

    // change camera signals
    QObject::connect(cameraButton, &QPushButton::clicked, modifier,
                     &GraphModifier::changePresetCamera);
    QObject::connect(zoomToSelectedButton, &QPushButton::clicked, modifier,
                     &GraphModifier::zoomToSelectedBar);

    // background & grid visible signals
    QObject::connect(backgroundCheckBox, &QCheckBox::stateChanged, modifier,
                     &GraphModifier::setBackgroundEnabled);
    QObject::connect(gridCheckBox, &QCheckBox::stateChanged, modifier,
                     &GraphModifier::setGridEnabled);
    QObject::connect(modifier, &GraphModifier::backgroundEnabledChanged,
                     backgroundCheckBox, &QCheckBox::setChecked);
    QObject::connect(modifier, &GraphModifier::gridEnabledChanged,
                     gridCheckBox, &QCheckBox::setChecked);

    // font size change signals
    QObject::connect(fontSizeSlider, &QSlider::valueChanged, modifier,
                     &GraphModifier::changeFontSize);
    QObject::connect(modifier, &GraphModifier::fontSizeChanged, fontSizeSlider,
                     &QSlider::setValue);

    // axis lables change signals
    QObject::connect(axisTitlesVisibleCB, &QCheckBox::stateChanged, modifier,
                     &GraphModifier::setAxisTitleVisibility);
    QObject::connect(axisLabelRotationSlider, &QSlider::valueChanged, modifier,
                     &GraphModifier::changeLabelRotation);
    //! [3]
    widget->show();
    return app.exec();
    //! [3]
}
