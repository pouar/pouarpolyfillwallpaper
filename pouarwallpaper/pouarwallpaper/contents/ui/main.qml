
import QtQuick 2.5
import org.kde.plasma.core 2.0 as PlasmaCore
import PouarQuick 1.0
import Qt.labs.folderlistmodel 2.1
import org.kde.plasma.wallpapers.image 2.0 as Wallpaper
import org.kde.kquickcontrolsaddons 2.0

Rectangle {
	id: root
	color: "#4F0000"
	Timer {
		interval: 600000; running: true; repeat: true
		onTriggered: action_next()
	}
	function action_next() {
		imagetmp.source = PouarQuick.randomfile("file:///home/pouar/Pictures/wp")
		image.sourceSize.width=(imagetmp.sourceSize.width>image.sourceSize.height) ? root.width : -1
		image.sourceSize.height=(imagetmp.sourceSize.height>image.sourceSize.width) ? root.height : -1
		image2.source = (image.source!=undefined) ? image.source : imagetmp.source
		if(issvg(imagetmp.source))
		{
		image.source = "image://image/"+imagetmp.source
		}
		else
		{
			image.source = imagetmp.source
		}
		animateImage.start()
	}

	function action_open() {
		Qt.openUrlExternally(imagetmp.source)
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

	function randomNumber() {
		return Math.random() * folderModel.count;
	}
	Component.onCompleted: {
		wallpaper.setAction("open", i18nd("plasma_applet_org.kde.image", "Open Wallpaper Image"), "document-open");
		wallpaper.setAction("next", i18nd("plasma_applet_org.kde.image","Next Wallpaper Image"),"user-desktop");
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
		mipmap : true
		fillMode: Image.PreserveAspectFit
		width:parent.width
		height:parent.height
	}
	Image {
		id: image2
		cache:false
		mipmap : true
		fillMode: Image.PreserveAspectFit
		width:parent.width
		height:parent.height
	}
	Image {
		id: imagetmp
		visible:false
		cache:false
	}
}