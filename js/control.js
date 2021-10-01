document.getElementById("btn_forward").onclick = fn_forward;
function fn_forward() {
  document.getElementById("dt_status").value = "Forward";
}

document.getElementById("btn_backward").onclick = fn_backward;
function fn_backward() {
  document.getElementById("dt_status").value = "Backward";
}

document.getElementById("btn_left").onclick = fn_left;
function fn_left() {
  document.getElementById("dt_status").value = "Turn Left";
}

document.getElementById("btn_right").onclick = fn_right;
function fn_right() {
  document.getElementById("dt_status").value = "Turn Right";
}

document.getElementById("send_time").onclick = fn_send_time;
function fn_send_time() {
  document.getElementById("dt_time").value =
    document.getElementById("pr_time").value;
}

$(document).ready(function () {
  document.getElementById("submit").onclick = fn_submit;
  function fn_submit() {
    $.post("./php/addData.php", $("#form").serialize(), function () {
      console.log("data sent");
    });
    return false;
  }
});
