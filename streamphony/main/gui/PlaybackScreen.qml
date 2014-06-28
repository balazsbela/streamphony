import QtQuick 2.2

Rectangle {
    id: playbackScreen
    width: 339; height: 102
    color: "#333333"

    property alias time: timeStr.text

    signal seekMusic(variant time)
    signal playPause
    signal next
    signal previous

    function changePlayPauseIcon(currentIcon){
        buttonPanel.changePlayPauseIcon(currentIcon)
    }

    function updateTotalTime(total){
        progressSlider.total = total
    }

    function updateProgressSlider(elapsed){
        progressSlider.update(elapsed)
    }

    function changeCurrentMusic(title, artist, album, path){
        topBox.title = title
        topBox.artist = artist
        topBox.album = album
    }

    function updateTimeElapsed(elapsed) {
        updateProgressSlider(elapsed);

        var totalSeconds = elapsed / 1000;

        var minutes = Math.floor(totalSeconds / 60);
        var seconds = Math.floor(totalSeconds % 60);

        var secondString = seconds < 10 ? "0" + seconds : seconds;
        time = minutes + ":" + secondString;
    }

    Rectangle {
        id: topBox

        property string title: ""
        property string artist: ""
        property string album: ""

        width: 341; height: 102
        color: "#212121"
        anchors { top: parent.top; topMargin: 0; left: parent.right; leftMargin: -339 }
        radius: 4

        Text {
            id: artist
            width: 340
            color: "#FFFFFF"
            text: topBox.artist == "" ? "" : topBox.artist
            font { family: "Univers LT Std"; pixelSize: 13 }
            anchors { top: topBox.top; topMargin: 2; left: topBox.left; leftMargin: 5 }
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: songTitle
            width: 340
            color: "#FFFFFF"
            text: topBox.title == "" ? "" : topBox.title
            font { family: "Univers LT Std"; pixelSize: 15 }
            anchors { top: artist.bottom; topMargin: 5; left: topBox.left; leftMargin: 5 }
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: album
            width: 340
            color: "#FFFFFF"
            text: topBox.album == "" ? "" : topBox.album
            font { family: "Univers LT Std"; pixelSize: 13 }
            anchors { top: songTitle.bottom; topMargin: 2; left: topBox.left; leftMargin: 5 }
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: timeStr
            y: 64
            width: 22
            color: "#FFFFFF"
            text: "0:00"
            font { family: "Univers LT Std"; pixelSize: 10 }
            anchors { bottom: topBox.bottom; bottomMargin: 28; left: topBox.left; leftMargin: 308 }
        }

        ButtonPanel {
            id: buttonPanel
            y: 63
            width: 100
            height: 28
            anchors { left: topBox.right; leftMargin: -336;}
            onPlayPausePressed: playbackScreen.playPause()
            onNextPressed: playbackScreen.next()
            onPreviousPressed: playbackScreen.previous()
        }

        ProgressBar {
            id: progressSlider
            x: 111
            height:1
            width: 209
            anchors.right: buttonPanel.left; anchors.rightMargin: -313; anchors.bottom: parent.bottom
            anchors.bottomMargin: 11
            anchors.top: parent.top
            anchors.topMargin: 83
            onSeek: {
                _mediaPlayer.seek(current);
                playbackScreen.seekMusic(current);
            }
        }

        Image {
            id: volumeIcons
            x: 111
            width: 196
            height: 14
            anchors { top: parent.top; topMargin: 63 }
            source: "icons/volume.png"
        }

        ProgressBar {
            id: volumeSlider
            x: 127
            y: 67
            width: 159; height: 4
            barWidth: 120 // static - TODO: implement the action
        }
    }

    Connections {
        target: _mediaPlayer
        onTimerPercentage : {
           // updateProgressSlider(percentage);
        }
        onCurrentTrackChanged : {
            topBox.title = title;
        }
        onTimerMilliSeconds : {
           updateTimeElapsed(msElapsed);
        }
        onTotalTimeChanged : {
            updateTotalTime(totalTimeMs);
        }
        onMetaDataReceived : {
           changeCurrentMusic(title, artist, album);
        }
    }
}
