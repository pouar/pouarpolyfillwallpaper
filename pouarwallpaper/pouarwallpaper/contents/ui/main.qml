
import QtQuick 2.5
import org.kde.plasma.core 2.0 as PlasmaCore
import PouarQuick 1.0
import Qt.labs.folderlistmodel 2.1
import org.kde.plasma.wallpapers.image 2.0 as Wallpaper
import org.kde.kquickcontrolsaddons 2.0
import QtQuick.Dialogs 1.2

Rectangle {
	id: root
	color: "#4F0000"
	property string imagetmp
	property string newimage
	property string oldimage
	readonly property string path: "/mnt/win7backup/wp"
	Timer {
		id:timer
		interval: 600000
		running: true
		repeat: true
		onTriggered: action_next()
	}
	function fileSelected(picture)
	{
		imagetmp = picture
		oldimage = (image.source!=undefined) ? newimage : imagetmp
		newimage=imagetmp;
		image.source = (issvg(newimage)) ? "image://image/"+newimage: newimage
		image2.source = (issvg(oldimage )) ? "image://image/"+oldimage : oldimage
		animateImage.start()
		timer.restart()
	}
	function action_next() {
		imagetmp = PouarQuick.randomfile("file://"+path)
		oldimage = (image.source!=undefined) ? newimage : imagetmp
		newimage=imagetmp;
		image.source = (issvg(newimage)) ? "image://image/"+newimage: newimage
		image2.source = (issvg(oldimage )) ? "image://image/"+oldimage : oldimage
		animateImage.start()
	}
	function action_select() {
			var window    = fileDialogCom.createObject(root)
			window.clicked.connect(fileSelected)

	}

	function action_open() {
		Qt.openUrlExternally(imagetmp)
	}
	
	function action_gc() {
		Qt.gc()
	}
	function sendTo() {
		return true;
	}
	function issvg(path) {
		var patt = /\.(svg|svgz)$/i;
		if(patt.test(path))
			return true;
		else
			return false;
	}
	Component.onCompleted: {
		wallpaper.setAction("open", i18nd("plasma_applet_org.kde.image", "Open Wallpaper Image"), "document-open");
		wallpaper.setAction("next", i18nd("plasma_applet_org.kde.image","Next Wallpaper Image"),"user-desktop");
		wallpaper.setAction("select", i18nd("plasma_applet_org.kde.image", "Select Wallpaper"), "preferences-desktop-wallpaper");
		wallpaper.setAction("gc", i18nd("plasma_applet_org.kde.image", "Run Garbage Collector"), "user-trash-full");
		action_next()
	}
	NumberAnimation {
		id: animateImage
		target: image
		properties: "opacity"
		from: 0.0
		to: 1.0
		duration:500
		easing {type: Easing.Linear}
		onStarted:animateImage2.start()
	}
	NumberAnimation {
		id: animateImage2
		target: image2
		properties: "opacity"
		from: 1.0
		to: 0.0
		duration:500
		easing {type: Easing.Linear}
	}
	Image {
		id: image
		cache:false
		fillMode: Image.PreserveAspectFit
		width:parent.width
		height:parent.height
		sourceSize.width:parent.width
		sourceSize.height:parent.height
	}
	Image {
		id: image2
		cache:false
		fillMode: Image.PreserveAspectFit
		width:parent.width
		height:parent.height
		sourceSize.width:parent.width
		sourceSize.height:parent.height
	}
	Component
	{
		id:fileDialogCom
		FileDialog {
			id: fileDialog
			title: "Pouar's Polyfill: Select A Wallpaper"
			folder: path
			property string picture;
			signal clicked(string picture);
			onAccepted: {
				fileDialog.clicked(fileDialog.fileUrl)
				fileDialog.destroy();
			}
			onRejected: {
				console.log("Canceled")
				fileDialog.destroy();
			}
			Component.onCompleted: visible = true
		}
	}
}
