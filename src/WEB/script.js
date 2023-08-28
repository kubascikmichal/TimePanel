setInterval(getStatus, 5000);

function setValues() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "setValues", true);
    xhttp.setRequestHeader('Content-Type', 'application/json');
    xhttp.send(JSON.stringify({
        university: document.getElementById("university").value,
        time: document.getElementById("timeInterval").value
    }));

    xhttp.onreadystatechange = function () {
        if (this.readyState == 4) {
            var response = JSON.parse(this.responseText);
            console.log(response);
            if (response.status == 200) {
                console.log('Settings uploaded');
            } else {
                console.log('Settings are not uploaded');
            }
        }
    }
}

function start() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "startClocks", true);
    xhttp.setRequestHeader('Content-Type', 'application/json');
    xhttp.send();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4) {
            console.log("start")
        }
    }
}

function reset() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "resetClocks", true);
    xhttp.setRequestHeader('Content-Type', 'application/json');
    xhttp.send();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4) {
            console.log("reset")
        }
    }
}
function stop() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "stopClocks", true);
    xhttp.setRequestHeader('Content-Type', 'application/json');
    xhttp.send();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4) {
            console.log("stop")
        }
    }
}

function sliderChange() {
    console.log(document.getElementById("myRange").value);
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "setBrightness", true);
    xhttp.setRequestHeader('Content-Type', 'application/json');
    xhttp.send(JSON.stringify({
        brightness: document.getElementById("myRange").value,
    }));

    xhttp.onreadystatechange = function () {
        if (this.readyState == 4) {
            console.log("set")
        }
    }
}


function getStatus() {
    document.getElementById("onlineStatus").style = "background-color: red;"
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "getStatus", true);
    xhttp.send();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4) {
            if (this.responseText != "") {
                var response = JSON.parse(this.responseText);
                if (response.status == 200) {
                    document.getElementById("onlineStatus").style = "background-color: green;"
                } else {
                    document.getElementById("onlineStatus").style = "background-color: red;"
                }
            }

        }
    }
}

function sync() {
    var xhttp = new XMLHttpRequest();
    var date = new Date();
    xhttp.open("POST", "syncRTC", true);
    xhttp.setRequestHeader('Content-Type', 'application/json');
    xhttp.send(JSON.stringify({
        day: date.getDate(),
        hours: date.getHours(),
        minutes: date.getMinutes(),
        seconds:date.getSeconds(),
        isConf: (date.getMonth() == 9 && date.getDate() >= 5 && date.getDate() <= 7)
    }));
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4) {
            var response = JSON.parse(this.responseText);
            console.log(response);
            if (response.status == 200) {
                console.log('Settings uploaded');
            } else {
                console.log('Settings are not uploaded');
            }
        }
    }
}

function programMode() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "programMode", true);
    xhttp.setRequestHeader('Content-Type', 'application/json');
    xhttp.send();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4) {
            console.log("changeMode")
        }
    }
}

function clocksMode() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "clocksMode", true);
    xhttp.setRequestHeader('Content-Type', 'application/json');
    xhttp.send();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4) {
            console.log("changeMode")
        }
    }
}