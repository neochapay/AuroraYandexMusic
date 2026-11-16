import QtQuick 2.0
import Sailfish.Silica 1.0

import QtMultimedia 5.6
import QtGraphicalEffects 1.0

Rectangle{
    id: bigPlayer
    width: parent.width
    height: parent.height
    color: {
        var track = currentPlayListModel.getCurrentTrack();
        if(track && track.derivedColors && track.derivedColors.average && track.derivedColors.average !== "") {
            return track.derivedColors.average
        }
        return "black"
    }

    property var currentTrack: null
    property color accentColor: (currentTrack && currentTrack.derivedColors && currentTrack.derivedColors.accent !== "") ? currentTrack.derivedColors.accent : Theme.highlightColor
    property color waveTextColor: (currentTrack && currentTrack.derivedColors && currentTrack.derivedColors.waveText !== "") ? currentTrack.derivedColors.waveText : Theme.primaryColor
    property string backgroundVideoSource: (currentTrack && currentTrack.backgroundVideoUri) ? currentTrack.backgroundVideoUri : ""
    property var stationDetector: null
    property var stationController: null
    property bool playOnStation: false

    function formatTime(ms) {
        if (ms <= 0 || isNaN(ms)) {
            return "0:00"
        }
        var totalSeconds = Math.floor(ms / 1000)
        var minutes = Math.floor(totalSeconds / 60)
        var seconds = totalSeconds % 60
        return minutes + ":" + (seconds < 10 ? "0" : "") + seconds
    }

    MediaPlayer {
        id: backgroundVideoPlayer
        source: bigPlayer.backgroundVideoSource
        autoLoad: true
        loops: MediaPlayer.Infinite
        muted: true

        function syncWithTrack() {
            if (source !== "" && rootAudio.playbackState === MediaPlayer.PlayingState) {
                if (playbackState !== MediaPlayer.PlayingState) {
                    play()
                }
            } else {
                if (playbackState === MediaPlayer.PlayingState) {
                    pause()
                }
            }
        }

        onSourceChanged: syncWithTrack()
        onStatusChanged: syncWithTrack()
        onPlaybackStateChanged: syncWithTrack()
    }

    Connections {
        target: rootAudio
        onPlaybackStateChanged: backgroundVideoPlayer.syncWithTrack()
    }

    VideoOutput {
        id: backgroundVideoOutput
        anchors.fill: parent
        source: backgroundVideoPlayer
        fillMode: VideoOutput.PreserveAspectCrop
        visible: bigPlayer.backgroundVideoSource !== "" && backgroundVideoPlayer.playbackState === MediaPlayer.PlayingState
        z: 0
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.25
        visible: bigPlayer.backgroundVideoSource !== "" && backgroundVideoPlayer.playbackState === MediaPlayer.PlayingState
        z: 1
    }

    MouseArea {
        id: swipeDownCloseArea
        anchors.fill: parent
        z: 1
        propagateComposedEvents: true
        property real pressX: 0
        property real pressY: 0
        property bool draggingDown: false

        onPressed: {
            pressX = mouse.x
            pressY = mouse.y
            draggingDown = false
        }

        onPositionChanged: {
            var dx = mouse.x - pressX
            var dy = mouse.y - pressY
            if (!draggingDown && dy > 20 && Math.abs(dy) > Math.abs(dx)) {
                draggingDown = true
            }
        }

        onReleased: {
            var dx = mouse.x - pressX
            var dy = mouse.y - pressY
            if (draggingDown && dy > 120 && Math.abs(dy) > Math.abs(dx)) {
                if (mainPlayer) { mainPlayer.height = Theme.itemSizeLarge }
            }
            draggingDown = false
        }

        onClicked: mouse.accepted = false
    }

    function applyRemoteTrackInfo() {
        if (!playOnStation || !stationController) {
            return
        }
        if (stationController.remoteTrackTitle && stationController.remoteTrackTitle.length > 0) {
            bigPlayerTitleLabel.text = stationController.remoteTrackTitle
        }
        if (stationController.remoteTrackArtist && stationController.remoteTrackArtist.length > 0) {
            bigPlayerArtistLabel.text = stationController.remoteTrackArtist
        }
        if (stationController.remoteTrackCover && stationController.remoteTrackCover.length > 0) {
            bigPlayerTrackCover.source = stationController.remoteTrackCover
        }
    }

    Connections{
        target: currentPlayListModel
        onCurrentIndexChanged: {
            if(currentPlayListModel.rowCount > 0) {
                var track = currentPlayListModel.getCurrentTrack();
                bigPlayer.currentTrack = track
                if(track == null) {
                    return
                }
                bigPlayerTitleLabel.text = track.title
                bigPlayerArtistLabel.text = track.artists[0].name
                if(track.albums[0].coverUri != "") {
                    bigPlayerTrackCover.source = "https://"+track.albums[0].coverUri.replace("%%", "400x400")
                }
                if(track.artists[0].coverUri != "") {
                    bigPlayerInfoItemArtistCover.source = "https://"+track.artists[0].coverUri.replace("%%", "100x100")
                }
            } else {
                bigPlayer.currentTrack = null
            }
            applyRemoteTrackInfo()
        }
    }

    Connections {
        target: stationController
        onRemoteTrackChanged: applyRemoteTrackInfo()
    }

    onPlayOnStationChanged: applyRemoteTrackInfo()

    Component.onCompleted: {
        bigPlayer.currentTrack = currentPlayListModel.getCurrentTrack()
    }

    IconButton {
        id: bigPlayerCloseButton
        width: Theme.itemSizeMedium
        height: width
        z: 2

        anchors{
            top: parent.top
            topMargin: Theme.paddingLarge
            left: parent.left
            leftMargin: Theme.paddingLarge
        }

        icon.source: "image://theme/icon-m-cancel?#FFFFFF"
        onClicked: if (mainPlayer) { mainPlayer.height = Theme.itemSizeLarge }
    }

    IconButton {
        id: bigPlayerOutputButton
        width: Theme.itemSizeMedium
        height: width
        z: 2
        enabled: stationDetector && stationDetector.valid

        anchors{
            top: parent.top
            topMargin: Theme.paddingLarge
            right: parent.right
            rightMargin: Theme.paddingLarge
        }

        icon.source: playOnStation ? "image://theme/icon-m-device?" + Theme.highlightColor : "image://theme/icon-m-device?#FFFFFF"

        onClicked: {
            if (!stationDetector || !stationDetector.valid || !stationController) {
                return
            }
            if (playOnStation) {
                stationController.stopRemotePlayback()
                stationController.disconnectFromStation()
                playOnStation = false
            } else {
                var track = currentPlayListModel.getCurrentTrack()
                playOnStation = true
                if (typeof rootAudio !== "undefined") {
                    rootAudio.stop()
                }
                if (!stationController.connected) {
                    stationController.connectToStation(stationDetector.stantionAddress,
                                                       stationDetector.stantionPort,
                                                       stationDetector.stantionDeviceId,
                                                       stationDetector.stantionPlatform)
                }
                if (track) {
                    var albumId = 0
                    if (track.albums && track.albums.length > 0) {
                        albumId = track.albums[0].albumId
                    }
                    stationController.playTrackByIds(parseInt(track.trackId), albumId)
                }
            }
        }
    }

    Rectangle {
        id: bigPlayerTrackCoverContainer
        property bool showCover: !(bigPlayer.backgroundVideoSource !== "" && backgroundVideoPlayer.playbackState === MediaPlayer.PlayingState)
        width: bigPlayer.width - Theme.paddingLarge*2
        height: width
        radius: Theme.paddingLarge
        clip: true
        color: "transparent"
        z: 2
        visible: showCover

        anchors{
            top: bigPlayerOutputButton.bottom
            topMargin: Theme.paddingLarge
            left: parent.left
            leftMargin: Theme.paddingLarge
        }

        Image{
            id: bigPlayerTrackCover
            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
        }
    }

    Item {
        id: bigPlayerInfoItem
        height: Theme.itemSizeLarge
        width: bigPlayer.width - Theme.paddingLarge*2
        z: 2

        anchors{
            top: bigPlayerTrackCoverContainer.bottom
            topMargin: Theme.paddingLarge
            left: parent.left
            leftMargin: Theme.paddingLarge
        }

        Item {
            id: bigPlayerInfoItemArtistCoverContainer
            width: parent.height
            height: width

            anchors{
                top: parent.top
                left: parent.left
            }

            Image {
                id: bigPlayerInfoItemArtistCover
                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop
                visible: false
            }

            Rectangle {
                id: artistCoverMask
                anchors.fill: parent
                radius: width / 2
                visible: false
            }

            OpacityMask {
                anchors.fill: parent
                source: bigPlayerInfoItemArtistCover
                maskSource: artistCoverMask
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    var track = currentPlayListModel.getCurrentTrack()
                    if(track && track.artists.length > 0) {
                        var artist = track.artists[0]
                        var aId = artist.artistId !== undefined ? artist.artistId : artist.id
                        if (mainPlayer) { mainPlayer.height = Theme.itemSizeLarge }
                        pageStack.push(Qt.resolvedUrl("../pages/ArtistPage.qml"), { artistId: aId })
                    }
                }
            }
        }

        Item {
            id: bigPlayerTitleContainer
            anchors{
                left: bigPlayerInfoItemArtistCoverContainer.right
                leftMargin: bigPlayerInfoItemArtistCoverContainer.width / 4
                bottom: parent.verticalCenter
            }
            width: parent.width - bigPlayerInfoItemArtistCoverContainer.width - bigPlayerinfoButton.width - bigPlayerInfoItemArtistCoverContainer.width / 4 - Theme.paddingSmall
            height: parent.height / 2
            clip: true

            Label{
                id: bigPlayerTitleLabel
                y: 0
                height: parent.height
                color: bigPlayer.waveTextColor
                font.pixelSize: Theme.fontSizeLarge
                font.bold: true
                verticalAlignment: Text.AlignVCenter

                property bool shouldScroll: contentWidth > bigPlayerTitleContainer.width

                onTextChanged: {
                    x = 0
                    if (shouldScroll) {
                        titleScrollAnimation.restart()
                    } else {
                        titleScrollAnimation.stop()
                        x = 0
                    }
                }
                onContentWidthChanged: {
                    x = 0
                    if (shouldScroll) {
                        titleScrollAnimation.restart()
                    } else {
                        titleScrollAnimation.stop()
                        x = 0
                    }
                }

                SequentialAnimation on x {
                    id: titleScrollAnimation
                    running: bigPlayerTitleLabel.shouldScroll
                    loops: Animation.Infinite
                    PauseAnimation { duration: 1500 }
                    NumberAnimation {
                        from: 0
                        to: Math.min(0, bigPlayerTitleContainer.width - bigPlayerTitleLabel.contentWidth)
                        duration: Math.max(1500, (bigPlayerTitleLabel.contentWidth - bigPlayerTitleContainer.width) * 30)
                    }
                    PauseAnimation { duration: 1500 }
                    NumberAnimation {
                        to: 0
                        duration: Math.max(1500, (bigPlayerTitleLabel.contentWidth - bigPlayerTitleContainer.width) * 30)
                    }
                }
            }
        }

        Label{
            id: bigPlayerArtistLabel
            anchors{
                left: bigPlayerInfoItemArtistCoverContainer.right
                leftMargin: bigPlayerInfoItemArtistCoverContainer.width / 4
                top: parent.verticalCenter
            }
            width: bigPlayerTitleContainer.width
            height: parent.height / 2
            color: bigPlayer.accentColor
            verticalAlignment: Text.AlignTop
        }

        IconButton {
            id: bigPlayerinfoButton
            width: parent.height
            height: width

            anchors{
                top: parent.top
                right: parent.right
            }

            icon.source: "image://theme/icon-m-about?#FFFFFF"
            //onClicked: pageStack.push(Qt.resolvedUrl("pages/AboutTrackPage.qml"))
        }
    }

    Label {
        id: currentTimeLabel
        anchors.left: parent.left
        anchors.leftMargin: Theme.paddingLarge
        anchors.verticalCenter: bigPlayerProgressBar.verticalCenter
        z: 2
        color: bigPlayer.waveTextColor
        font.pixelSize: Theme.fontSizeExtraSmall
        text: {
            if (bigPlayer.playOnStation && stationController) {
                return bigPlayer.formatTime(stationController.remoteProgress * 1000)
            }
            return bigPlayer.formatTime(rootAudio.position)
        }
    }

    Label {
        id: totalTimeLabel
        anchors.right: parent.right
        anchors.rightMargin: Theme.paddingLarge
        anchors.verticalCenter: bigPlayerProgressBar.verticalCenter
        z: 2
        color: bigPlayer.waveTextColor
        font.pixelSize: Theme.fontSizeExtraSmall
        horizontalAlignment: Text.AlignRight
        text: {
            var track = currentPlayListModel.getCurrentTrack();
            if (bigPlayer.playOnStation && stationController && stationController.remoteDuration > 0) {
                return bigPlayer.formatTime(stationController.remoteDuration * 1000)
            }
            if(track && track.durationMs > 0) {
                return bigPlayer.formatTime(track.durationMs)
            }
            if(rootAudio.duration > 0) {
                return bigPlayer.formatTime(rootAudio.duration)
            }
            return "0:00"
        }
    }

    ProgressBar{
        id: bigPlayerProgressBar
        height: Theme.itemSizeLarge
        z: 2

        anchors{
            top: bigPlayerInfoItem.bottom
            topMargin: Theme.paddingLarge
            left: currentTimeLabel.right
            leftMargin: Theme.paddingSmall
            right: totalTimeLabel.left
            rightMargin: Theme.paddingSmall
        }
        minimumValue: 0
        indeterminate: {
            if (bigPlayer.playOnStation && stationController) {
                return stationController.remoteDuration <= 0
            }
            return rootAudio.duration <= 0
        }
        maximumValue: {
            if (bigPlayer.playOnStation && stationController) {
                return stationController.remoteDuration > 0 ? stationController.remoteDuration : 1.0
            }
            return rootAudio.duration > 0 ? rootAudio.duration : 1.0
        }
        value: {
            if (bigPlayer.playOnStation && stationController) {
                return stationController.remoteProgress
            }
            return rootAudio.duration > 0 ? rootAudio.position : 0
        }
        label: ""

        MouseArea{
            enabled: !bigPlayer.playOnStation && rootAudio.seekable
            anchors.fill: parent
            onClicked: {
                var pos = (mouseX - bigPlayerProgressBar.x)/bigPlayerProgressBar.width
                rootAudio.seek(pos * rootAudio.duration)
            }
        }
    }

    Item {
        id: bigPlayerControlsItem
        height: Theme.itemSizeLarge
        width: bigPlayer.width - Theme.paddingLarge*2
        z: 2

        anchors{
            top: bigPlayerProgressBar.bottom
            topMargin: Theme.paddingMedium
            left: parent.left
            leftMargin: Theme.paddingLarge
        }

        SvgIcon{
            id: bigPlayerDislikeButton
            width: parent.height*0.6
            height: width
            defaultColor: bigPlayer.waveTextColor
            activeColor: bigPlayer.accentColor

            source: "../img/heart-crack.svg"

            anchors{
                right: bigPlayerBackwardButton.left
                rightMargin: Theme.paddingLarge
                verticalCenter: parent.verticalCenter
            }
            onClicked: user.dislike("track", currentPlayListModel.getTrack(currentPlayListModel.currentIndex).trackId)
        }

        SvgIcon{
            id: bigPlayerBackwardButton
            width: parent.height*0.6
            height: width
            enabled:  currentPlayListModel.currentIndex > 0
            defaultColor: bigPlayer.waveTextColor
            activeColor: bigPlayer.accentColor

            source: "../img/backward.svg"

            anchors{
                right: bigPlayerPlayButton.left
                rightMargin: Theme.paddingLarge
                verticalCenter: parent.verticalCenter
            }
            onClicked: currentPlayListModel.currentIndex--
        }

        SvgIcon{
            id: bigPlayerPlayButton
            width: parent.height
            height: width
            defaultColor: bigPlayer.waveTextColor
            activeColor: bigPlayer.accentColor
            source: (bigPlayer.playOnStation ? (stationController && stationController.remotePlaying) : (rootAudio.playbackState == MediaPlayer.PlayingState)) ? "../img/pause.svg" : "../img/play.svg"
            anchors.centerIn: parent
            onClicked: {
                if (bigPlayer.playOnStation && stationController) {
                    if (stationController.remotePlaying) {
                        stationController.pauseRemotePlayback()
                    } else {
                        stationController.resumeRemotePlayback()
                    }
                } else {
                    if(rootAudio.playbackState == MediaPlayer.PlayingState) {
                        rootAudio.pause()
                    } else {
                        rootAudio.play()
                    }
                }
            }
        }

        SvgIcon{
            id: bigPlayerForwardButton
            width: parent.height*0.6
            height: width
            defaultColor: currentPlayListModel.nextReady ? bigPlayer.waveTextColor : "gray"
            activeColor: bigPlayer.accentColor
            enabled: currentPlayListModel.nextReady

            source: "../img/forward.svg"

            anchors{
                left: bigPlayerPlayButton.right
                leftMargin: Theme.paddingLarge
                verticalCenter: parent.verticalCenter
            }

            onClicked:{
                rootAudio.stop()
                currentPlayListModel.currentIndex++
            }
        }

        SvgIcon{
            id: bigPlayerLikeButton
            width: parent.height*0.6
            height: width
            defaultColor: bigPlayer.waveTextColor
            activeColor: bigPlayer.accentColor

            source: "../img/heart.svg"

            anchors{
                left: bigPlayerForwardButton.right
                leftMargin: Theme.paddingLarge
                verticalCenter: parent.verticalCenter
            }
            onClicked: user.like("track", currentPlayListModel.getTrack(currentPlayListModel.currentIndex).trackId)
        }
    }
}

