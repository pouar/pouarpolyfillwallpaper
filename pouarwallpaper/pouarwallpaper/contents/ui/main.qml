
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
	Timer {
		interval: 600000; running: true; repeat: true
		onTriggered: action_next()
	}
	function fileSelected(picture)
	{
		imagetmp.source = picture
		image.sourceSize.width=(imagetmp.sourceSize.width>image.sourceSize.height) ? root.width : -1
		image.sourceSize.height=(imagetmp.sourceSize.height>image.sourceSize.width) ? root.height : -1
		oldimage.text = (image.source!=undefined) ? newimage.text : imagetmp.source
		newimage.text=imagetmp.source;
		image.source = (issvg(newimage.text)) ? "image://image/"+newimage.text: newimage.text
		image2.source = (issvg(oldimage.text )) ? "image://image2/"+oldimage.text : oldimage.text
		animateImage.start()
	}
	function action_next() {
		imagetmp.source = PouarQuick.randomfile("file:///mnt/win7backup/wp")
		image.sourceSize.width=(imagetmp.sourceSize.width>image.sourceSize.height) ? root.width : -1
		image.sourceSize.height=(imagetmp.sourceSize.height>image.sourceSize.width) ? root.height : -1
		oldimage.text = (image.source!=undefined) ? newimage.text : imagetmp.source
		newimage.text=imagetmp.source;
		image.source = (issvg(newimage.text)) ? "image://image/"+newimage.text: newimage.text
		image2.source = (issvg(oldimage.text )) ? "image://image2/"+oldimage.text : oldimage.text
		animateImage.start()
	}
	function action_select() {
		var component = Qt.createComponent("dialog.qml")
			var window    = component.createObject(root)
			window.clicked.connect(fileSelected)

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
		wallpaper.setAction("select", i18nd("plasma_applet_org.kde.image", "Select Wallpaper"), "preferences-desktop-wallpaper");
		
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
		cache:true
		mipmap : true
		fillMode: Image.PreserveAspectFit
		width:parent.width
		height:parent.height
	}
	Image {
		id: image2
		cache:true
		mipmap : true
		fillMode: Image.PreserveAspectFit
		width:parent.width
		height:parent.height
	}
	Image {
		id: imagetmp
		visible:false
		cache:true
	}
	
	Text {
			id:oldimage
			text: ""
			visible:false
		}
	Text {
			id:newimage
			text: ""
			visible:false
		}
/*
	FileDialog {
		id: fileDialog
		title: "Please choose a file"
		folder: "file:///mnt/win7backup/wp"
		visible:false
		onAccepted: {
			visible = false
			imagetmp.source = fileUrls
			image.sourceSize.width=(imagetmp.sourceSize.width>image.sourceSize.height) ? root.width : -1
			image.sourceSize.height=(imagetmp.sourceSize.height>image.sourceSize.width) ? root.height : -1
			oldimage.text = (image.source!=undefined) ? newimage.text : imagetmp.source
			newimage.text=imagetmp.source;
			image.source = (issvg(newimage.text)) ? "image://image/"+newimage.text: newimage.text
			image2.source = (issvg(oldimage.text )) ? "image://image2/"+oldimage.text : oldimage.text
			animateImage.start()
			
		}
		onRejected: {
			
		}
		Component.onCompleted: visible = true
	}
	*/
}
