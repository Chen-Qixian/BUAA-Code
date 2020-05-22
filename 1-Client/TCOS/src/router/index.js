import Vue from 'vue'
import Router from 'vue-router'
import Home from '@/pages/home/Home'
import HomeEn from '@/pages/home_en/Home'
import Result from '@/pages/result/Result'

Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'Home',
      component: Home
    },
    {
      path: '/en',
      name: 'HomeEn',
      component: HomeEn
    },
    {
      path: '/result',
      name: 'Result',
      component: Result
    }
  ]
})
