var data1 = [];
var data2 = [];
var data3 = [];
var data4 = [];
var myTable = document.getElementById("datatablesSimple");

// load data tu database
$(document).ready(function () {
  update();
});
setInterval(update, 1000);
function update() {
  // gui request xuong database de lay data
  $.post("./php/getData.php", function (data) {
    var data1 = [];
    var data2 = [];
    var data3 = [];
    var data4 = [];
    for (var i in data) {
      data1.push(data[i].id);
      data2.push(data[i].user);
      data3.push(data[i].pass);
      data4.push(data[i].time);
      myTable.rows[Number(i) + 1].cells[0].textContent = data1[i];
      myTable.rows[Number(i) + 1].cells[1].textContent = data2[i];
      myTable.rows[Number(i) + 1].cells[2].textContent = data3[i];
      myTable.rows[Number(i) + 1].cells[3].textContent = data4[i];
    }
  });
}
