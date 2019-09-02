import './bootstrap';
window.Vue = require('vue');
Vue.component('chart-component', require('./components/ChartContainer.vue')["default"]);
var app = new Vue({
  el: '#app'
});
