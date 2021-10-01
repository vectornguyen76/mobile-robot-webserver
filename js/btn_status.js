// load data tu database
$(document).ready(function () {
  update();
});
setInterval(update, 1000);
function update() {
  // gui request xuong database de lay data
  $.post("./php/getStatus.php", function (data) {
    var dt_feedback = [];
    dt_feedback.push(data[0].status_feedback);
    if(dt_feedback[0] == "1")
    {
        document.getElementById("lb_result").innerHTML = "Connected";
        console.log("connect");
    }
    else if(dt_feedback[0] == "0")
    {
        document.getElementById("lb_result").innerHTML = "Disconnected";
        console.log("disconnect");
    }
  });
}





document.getElementById("connect").onclick = fn_connect;
function fn_connect() {
    console.log("data connect");
    document.getElementById("lb_result").innerHTML = "Connect successfully!";
    document.getElementById("lb_result").style.color = "#089163";

    var data = {
        status: "1"
    };
    $.post("./php/addStatus.php", data);

}
document.getElementById("disconnect").onclick = fn_disconnect;
function fn_disconnect() {
    console.log("data disconnect");
    document.getElementById("lb_result").innerHTML = "Disconnect successfully!";
    document.getElementById("lb_result").style.color = "red";

    var data = {
        status: "0"
    };
    $.post("./php/addStatus.php", data);
}

