let minutes;
let seconds;
let count = 0;
var interval;

function starttimer(x) {

    if (count >= 1) {
        clearInterval(interval);
    }

    minutes = x;
    seconds = 0;

    var element = document.getElementById("timer");

    interval = setInterval(
        function() {
            if (seconds == 0 && minutes == 0) {
                clearInterval(interval);
                document.title = "Time's UP"
                element.innerHTML = "00:00";
                element.style.backgroundColor = "red"
            }
            else if (seconds == 0) {
                minutes -= 1;
                seconds = 59;
                element.innerHTML = minutes + ":" + seconds;
                document.title = "PomoTimer " + minutes + ":" + seconds;
                count += 1;
            }
            else if (seconds > 0) {
                seconds -= 1;
                element.innerHTML = minutes + ":" + seconds
                document.title = "PomoTimer " + minutes + ":" + seconds;
            }
        }

    ,1000)

}