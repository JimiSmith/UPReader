import QtQuick 2.0

Rectangle {
	id: rootItem
	property alias text: waitText.text

	Text {
		id: waitText
		anchors.bottom: spinnerImage.top
		anchors.horizontalCenter: parent.horizontalCenter
		width: parent.width / 2
		wrapMode: Text.Wrap
		horizontalAlignment: Text.AlignHCenter
		text: "Please Wait";
	}

	AnimatedImage {
		id: spinnerImage
		anchors.centerIn: parent
		smooth: true
		source: "../images/spinner.gif"
	}
}
