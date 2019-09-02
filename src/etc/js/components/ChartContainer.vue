<template>
  <div class="container">
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
    chartdata: null
  }),
  async mounted() {
    this.loaded = false;
    axios
      .get("/api/statistics/count")
      .then((response) => {
        this.chartdata = response.data;
        this.loaded = true;
      })
      .catch(error => console.log(error));
  }
};
</script>
