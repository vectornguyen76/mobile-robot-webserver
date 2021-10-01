var stt = [];
var distance = [];

//var stt = [1,2,3,4,5,6,7,8,9];
//var distance = [1,3,4,6,3,5,2,10,3];
// Set new default font family and font color to mimic Bootstrap's default styling
Chart.defaults.global.defaultFontFamily =
  '-apple-system,system-ui,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif';
Chart.defaults.global.defaultFontColor = "#292b2c";

// Area Chart Example
var ctx = document.getElementById("mycanvas");
var myLineChart = new Chart(ctx, {
  type: "line",
  data: {
    labels: stt,
    datasets: [
      {
        label: "Distance",
        lineTension: 0.3,
        backgroundColor: "rgba(2,117,216,0.2)",
        borderColor: "rgba(2,117,216,1)",
        pointRadius: 5,
        pointBackgroundColor: "rgba(2,117,216,1)",
        pointBorderColor: "rgba(255,255,255,0.8)",
        pointHoverRadius: 5,
        pointHoverBackgroundColor: "rgba(2,117,216,1)",
        pointHitRadius: 50,
        pointBorderWidth: 2,
        data: distance,
      },
    ],
  },
  options: {
    animation: false,
    scales: {
      xAxes: [
        {
          time: {
            unit: "time",
          },
          gridLines: {
            display: false,
          },
          ticks: {
            maxTicksLimit: 10,
          },
        },
      ],
      yAxes: [
        {
          ticks: {
            min: 0,
            max: 110,
            maxTicksLimit: 22,
          },
          gridLines: {
            color: "rgba(0, 0, 0, .125)",
          },
        },
      ],
    },
    legend: {
      display: false,
    },
  },
});

// load data tu database
$(document).ready(function () {
  updateChart();
});

setInterval(updateChart, 1000);
function updateChart() {
  // gui request xuong database de lay data

  //chu y duong dan
  $.post("./php/followersdata.php", function (data) {
    var stt = [];
    var distance = [];

    for (var i in data) {
      stt.push(data[i].stt);
      distance.push(data[i].kcach);
    }
    myLineChart.data.labels = stt;
    myLineChart.data.datasets[0].data = distance;

    myLineChart.update();
  });
}
