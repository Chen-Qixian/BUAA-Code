/**
 * Author: Chen Qixian
 * Email: chenqixian@buaa.edu.cn
 */
import Vue from 'vue'
import App from './App'
import router from './router'
import fastClick from 'fastclick'
import 'jquery'
import 'ant-design-vue/dist/antd.css'
import { Icon, Button, InputNumber, Input, Select, Switch, Table, message, Spin, Modal, Dropdown, Menu, MenuItem } from 'ant-design-vue'
import ECharts from 'vue-echarts/components/ECharts.vue'
import store from './store'

Vue.use(Icon)
Vue.use(Button)
Vue.use(InputNumber)
Vue.use(Input)
Vue.use(Select)
Vue.use(Switch)
Vue.use(Table)
Vue.use(Spin)
Vue.use(Modal)
Vue.use(Dropdown)
Vue.use(Menu)
Vue.prototype.$message = message;

Vue.component('v-chart', ECharts)

Vue.config.productionTip = false
fastClick.attach(document.body)

new Vue({
  el: '#app',
  router,
  store,
  components: { App },
  template: '<App/>'
})
