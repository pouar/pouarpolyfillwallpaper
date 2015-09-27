import QtQuick 2.2
import QtQuick.Dialogs 1.0

FileDialog {
	id: fileDialog
	title: "Pouar's Polyfill: Select A Wallpaper"
	folder: "/mnt/win7backup/wp"
	property string picture;
	signal clicked(string picture);
	onAccepted: {
		fileDialog.clicked(fileDialog.fileUrl)
	}
	onRejected: {
		console.log("Canceled")
	}
	Component.onCompleted: visible = true
}