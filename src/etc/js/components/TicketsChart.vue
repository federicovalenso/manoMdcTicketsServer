<template>
  <line-chart-container :options="options" :api-route="apiRoute"></line-chart-container>
</template>

<script>
import LineChartContainer from "./LineChartContainer.vue";

export default {
  name: "TicketsChart",
  components: { LineChartContainer },
  data: () => ({
    options: {
      responsive: true,
      maintainAspectRatio: false,
      scales: {
        xAxes: [
          {
            scaleLabel: {
              display: true,
              labelString: "Часы"
            }
          }
        ],
        yAxes: [
          {
            scaleLabel: {
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
    apiRoute: "/api/statistics/count"
  })
};
</script>