#include "ControlWidget.h"
#include "MainWindow.h"
#include "GLWidget.h"
#include "GraphUtil.h"
#include "RoadSegmentationUtil.h"

ControlWidget::ControlWidget(MainWindow* mainWin) : QDockWidget("Control Widget", (QWidget*)mainWin) {
	this->mainWin = mainWin;

	// set up the UI
	ui.setupUi(this);

	// register the event handlers
	connect(ui.pushButtonDetectGrid, SIGNAL(clicked()), this, SLOT(detectGrid()));
	connect(ui.pushButtonDetectPlaza, SIGNAL(clicked()), this, SLOT(detectPlaza()));
	connect(ui.pushButtonDetectRadial, SIGNAL(clicked()), this, SLOT(detectRadial()));

	hide();
}

void ControlWidget::setRoadVertex(RoadVertexDesc vertexDesc, RoadVertexPtr selectedVertex) {
	QString desc("");
	QString location("");
	QString neighbors("");

	if (selectedVertex != NULL) {
		desc.setNum(vertexDesc);

		location = QString("(%1, %2)").arg(selectedVertex->pt.x(), 0, 'f', 0).arg(selectedVertex->pt.y(), 0, 'f', 0);

		std::vector<RoadVertexDesc> n = GraphUtil::getNeighbors(mainWin->glWidget->roads, vertexDesc);
		for (int i = 0; i < n.size(); i++) {
			QString str;
			str.setNum(n[i]);

			neighbors += str;
			if (i < n.size() - 1) neighbors += ",";
		}
	}

	ui.lineEditVertexDesc->setText(desc);
	ui.lineEditVertexPos->setText(location);
	ui.lineEditVertexNeighbors->setText(neighbors);
}

/**
 * Display the selected edge information.
 */
void ControlWidget::setRoadEdge(RoadEdgePtr selectedEdge) {
	QString type("");
	QString numLanes("");
	QString oneWay("");

	if (selectedEdge != NULL) {
		switch (selectedEdge->type) {
		case 3:
			type = "Highway";
			break;
		case 2:
			type = "Avenue";
			break;
		case 1:
			type = "Street";
			break;
		}

		numLanes.setNum(selectedEdge->lanes);

		if (selectedEdge->oneWay) {
			oneWay = "Yes";
		} else {
			oneWay = "No";
		}
	}

	ui.lineEditEdgeType->setText(type);
	ui.lineEditEdgeLanes->setText(numLanes);
	ui.lineEditEdgeOneWay->setText(oneWay);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event handlers

/**
 * Event handler for button [Detect Grid]
 */
void ControlWidget::detectGrid() {
	RoadSegmentationUtil::detectGrid(mainWin->glWidget->roads);

	mainWin->glWidget->updateGL();
}

/**
 * Event handler for button [Detect Plaza]
 */
void ControlWidget::detectPlaza() {
	RoadSegmentationUtil::detectPlaza(mainWin->glWidget->roads);

	mainWin->glWidget->updateGL();
}

/**
 * Event handler for button [Detect Radial]
 */
void ControlWidget::detectRadial() {
	RoadSegmentationUtil::detectRadial(mainWin->glWidget->roads);

	mainWin->glWidget->updateGL();
}


