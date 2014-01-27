#include "ControlWidget.h"
#include "MainWindow.h"
#include "GLWidget.h"
#include "GraphUtil.h"
#include "RoadSegmentationUtil.h"

ControlWidget::ControlWidget(MainWindow* mainWin) : QDockWidget("Control Widget", (QWidget*)mainWin) {
	this->mainWin = mainWin;

	// set up the UI
	ui.setupUi(this);
	ui.lineEditGridMaxIteration->setText("6");
	ui.lineEditNumBins->setText("9");
	ui.lineEditMinTotalLength->setText("1000");
	ui.lineEditMinMaxBinRatio->setText("0.5");
	ui.lineEditGridVotingThreshold->setText("0.7");

	ui.lineEditRadialMaxIteration->setText("2");
	ui.lineEditScale1->setText("0.05");
	ui.lineEditScale2->setText("0.1");
	ui.lineEditCenterErrorTol2->setText("200");
	ui.lineEditAngleThreshold2->setText("0.4");
	ui.lineEditScale3->setText("0.2");
	ui.lineEditCenterErrorTol3->setText("200");
	ui.lineEditAngleThreshold3->setText("0.2");
	ui.lineEditRadialVotingThreshold->setText("0.7");
	ui.lineEditRadialSeedDistance->setText("200");
	ui.lineEditRadialExtendingAngleThreshold->setText("0.2");

	// register the event handlers
	connect(ui.pushButtonDetectGrid, SIGNAL(clicked()), this, SLOT(detectGrid()));
	connect(ui.pushButtonDetectPlaza, SIGNAL(clicked()), this, SLOT(detectPlaza()));
	connect(ui.pushButtonDetectRadial, SIGNAL(clicked()), this, SLOT(detectRadial()));
	connect(ui.pushButtonDetectGridRadial, SIGNAL(clicked()), this, SLOT(detectGridRadial()));

	hide();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event handlers

/**
 * Event handler for button [Detect Grid]
 */
void ControlWidget::detectGrid() {
	int maxIteration = ui.lineEditGridMaxIteration->text().toInt();
	int numBins = ui.lineEditNumBins->text().toInt();
	float minTotalLength = ui.lineEditMinTotalLength->text().toFloat();
	float minMaxBinRatio = ui.lineEditMinMaxBinRatio->text().toFloat();
	float votingThreshold = ui.lineEditGridVotingThreshold->text().toFloat();

	GraphUtil::copyRoads(mainWin->glWidget->origRoads, mainWin->glWidget->roads);
	RoadSegmentationUtil::detectGrid(mainWin->glWidget->roads, mainWin->glWidget->selectedArea, maxIteration, numBins, minTotalLength, minMaxBinRatio, votingThreshold);

	mainWin->glWidget->updateGL();
}

/**
 * Event handler for button [Detect Plaza]
 */
void ControlWidget::detectPlaza() {
	GraphUtil::copyRoads(mainWin->glWidget->origRoads, mainWin->glWidget->roads);
	RoadSegmentationUtil::detectPlaza(mainWin->glWidget->roads, mainWin->glWidget->selectedArea);

	mainWin->glWidget->updateGL();
}

/**
 * Event handler for button [Detect Radial]
 */
void ControlWidget::detectRadial() {
	int maxIteration = ui.lineEditRadialMaxIteration->text().toInt();
	float scale1 = ui.lineEditScale1->text().toFloat();
	float scale2 = ui.lineEditScale2->text().toFloat();
	float centerErrorTol2 = ui.lineEditCenterErrorTol2->text().toFloat();
	float angleThreshold2 = ui.lineEditAngleThreshold2->text().toFloat();
	float scale3 = ui.lineEditScale3->text().toFloat();
	float centerErrorTol3 = ui.lineEditCenterErrorTol3->text().toFloat();
	float angleThreshold3 = ui.lineEditAngleThreshold3->text().toFloat();
	float votingThreshold = ui.lineEditRadialVotingThreshold->text().toFloat();
	float seedDistance = ui.lineEditRadialSeedDistance->text().toFloat();
	float extendingAngleThreshold = ui.lineEditRadialExtendingAngleThreshold->text().toFloat();

	GraphUtil::copyRoads(mainWin->glWidget->origRoads, mainWin->glWidget->roads);
	RoadSegmentationUtil::detectRadial(mainWin->glWidget->roads, mainWin->glWidget->selectedArea, maxIteration, scale1, scale2, centerErrorTol2, angleThreshold2, scale3, centerErrorTol3, angleThreshold3, votingThreshold, seedDistance, extendingAngleThreshold);

	mainWin->glWidget->updateGL();
}

void ControlWidget::detectGridRadial() {
	int gridMaxIteration = ui.lineEditGridMaxIteration->text().toInt();
	int numBins = ui.lineEditNumBins->text().toInt();
	float minTotalLength = ui.lineEditMinTotalLength->text().toFloat();
	float minMaxBinRatio = ui.lineEditMinMaxBinRatio->text().toFloat();
	float gridVotingThreshold = ui.lineEditGridVotingThreshold->text().toFloat();

	int radialMaxIteration = ui.lineEditRadialMaxIteration->text().toInt();
	float scale1 = ui.lineEditScale1->text().toFloat();
	float scale2 = ui.lineEditScale2->text().toFloat();
	float centerErrorTol2 = ui.lineEditCenterErrorTol2->text().toFloat();
	float angleThreshold2 = ui.lineEditAngleThreshold2->text().toFloat();
	float scale3 = ui.lineEditScale3->text().toFloat();
	float centerErrorTol3 = ui.lineEditCenterErrorTol3->text().toFloat();
	float angleThreshold3 = ui.lineEditAngleThreshold3->text().toFloat();
	float radialVotingThreshold = ui.lineEditRadialVotingThreshold->text().toFloat();
	float seedDistance = ui.lineEditRadialSeedDistance->text().toFloat();
	float extendingAngleThreshold = ui.lineEditRadialExtendingAngleThreshold->text().toFloat();


	GraphUtil::copyRoads(mainWin->glWidget->origRoads, mainWin->glWidget->roads);
	RoadSegmentationUtil::detectRadial(mainWin->glWidget->roads, mainWin->glWidget->selectedArea, radialMaxIteration, scale1, scale2, centerErrorTol2, angleThreshold2, scale3, centerErrorTol3, angleThreshold3, radialVotingThreshold, seedDistance, extendingAngleThreshold);
	RoadSegmentationUtil::detectGrid(mainWin->glWidget->roads, mainWin->glWidget->selectedArea, gridMaxIteration, numBins, minTotalLength, minMaxBinRatio, gridVotingThreshold);

	mainWin->glWidget->updateGL();
}

