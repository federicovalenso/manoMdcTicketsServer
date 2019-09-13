import './bootstrap';
import { TabsPlugin } from 'bootstrap-vue';
window.Vue = require('vue');
Vue.component('tickets-chart', require('./components/TicketsChart.vue')["default"]);
Vue.use(TabsPlugin);
var app = new Vue({
  el: '#app'
});
