<template>
  <div class="container">
    <div class="form-group row">
      <div class="form-group col-md-4">
        <label for="dateFrom">С:</label>
        <input type="date" id="dateFrom" class="form-control" v-model="dateFrom" />
      </div>
      <div class="form-group col-md-4">
        <label for="dateTo">По:</label>
        <input type="date" id="dateTo" class="form-control" v-model="dateTo" />
      </div>
    </div>
    <line-chart v-if="loaded" :chartdata="chartdata" :options="options" />
  </div>
</template>

<script>
import LineChart from "./LineChart.vue";
import axios from "axios";

export default {
  name: "LineChartContainer",
  components: { LineChart },
  data: () => ({
    loaded: false,
    chartdata: null,
    options: {
      responsive: true,
      maintainAspectRatio: false,
      scales: {
        xAxes: [{
          scaleLabel : {
              display: true,
              labelString: "Часы"
            }
        }
        ],
        yAxes: [
          {
            scaleLabel : {
              display: true,
              labelString: "Талоны"
            },
            ticks: {
              suggestedMin: 0
            }
          }
        ]
      }
    },
    dateFrom: null,
    dateTo: null
  }),
  async mounted() {
    this.dateFrom = this.currentDate();
    this.dateTo = this.dateFrom;
    this.updateChart();
  },
  methods: {
    currentDate() {
      let dateFormat = require("dateformat");
      return dateFormat(Date(), "yyyy-mm-dd");
    },
    updateChart() {
      this.loaded = false;
      const dateFrom = Date.parse(this.dateFrom);
      const dateTo = Date.parse(this.dateTo);
      if (dateFrom > dateTo) {
        this.dateTo = this.dateFrom;
      } else if (dateTo < dateFrom) {
        this.dateFrom = this.dateTo;
      }
      axios
        .get("/api/statistics/count", {
          params: { from: this.dateFrom, to: this.dateTo }
        })
        .then(response => {
          this.chartdata = response.data;
          this.loaded = true;
        })
        .catch(error => console.log(error));
    }
  },
  watch: {
    dateFrom: "updateChart",
    dateTo: "updateChart"
  }
};
</script>
