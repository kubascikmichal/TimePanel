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