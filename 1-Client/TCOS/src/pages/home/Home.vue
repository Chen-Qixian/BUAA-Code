<template>
  <div>
    <home-header :cn="chinese"></home-header>
    <home-navbar @change="navChange" :itemList="itemList" :navId="nav" :cn="chinese"></home-navbar>
    <home-body :nav="nav">
      <body-quick v-if="nav === QUICK" @postPara="handlePost"></body-quick>
      <body-lpms v-else-if="nav === LPMS" @postPara="handlePost" :paraStatus="paraSucc"></body-lpms>
      <body-qtls v-else-if="nav === QTLS" @postPara="handlePost" :paraStatus="paraSucc"></body-qtls>
      <body-topomapping v-else-if="nav === TOPOMAPPING" @postPara="handlePost" :paraStatus="paraSucc"></body-topomapping>
      <body-aphid v-else-if="nav === APHID" @postPara="handlePost" :paraStatus="paraSucc"></body-aphid>
      <body-treematch v-else-if="nav === TREEMATCH" @postPara="handlePost"></body-treematch>
      <body-mpipp v-else-if="nav === MPIPP" @postPara="handlePost"></body-mpipp>
    </home-body>
    <home-footer></home-footer>
  </div>
</template>

<script>
import HomeHeader from '../common/Header'
import HomeNavbar from '../common/Navbar' 
import HomeBody from '../common/Body'
import HomeFooter from '../common/Footer'

import BodyQuick from './components/Quick'
import BodyLpms from './components/LPMS'
import BodyQtls from './components/QTLS'
import BodyTopomapping from './components/Topo'
import BodyAphid from './components/APHiD'
import BodyTreematch from './components/Tree'
import BodyMpipp from './components/MPIPP'

import axios from 'axios'
import { mapMutations } from 'vuex'

export default {
  name: 'Home',
  components: {
    HomeHeader,
    HomeNavbar,
    HomeBody,
    HomeFooter,
    BodyQuick,
    BodyLpms,
    BodyQtls,
    BodyTopomapping,
    BodyAphid,
    BodyTreematch,
    BodyMpipp
  },
  data () {
    return {
      chinese: true,
      nav: 0,
      itemList: [
        {
          type: 'thunderbolt',
          msg:'快速开始'
        },
        {
          type: 'interation',
          msg:'使用LPMS'
        },
        {
          type: 'block',
          msg:'使用QTLS'
        },
        {
          type: 'deployment-unit',
          msg:'使用TopoMapping'
        },
        {
          type: 'branches',
          msg:'使用APHiD'
        },
        {
          type: 'cluster',
          msg:'使用TreeMatch'
        },
        {
          type: 'gateway',
          msg:'使用MPIPP'
        }
      ],
      QUICK: 0,
      LPMS: 1,
      QTLS: 2,
      TOPOMAPPING: 3,
      APHID: 4,
      TREEMATCH: 5,
      MPIPP: 6,
      paraSucc: false
    }
  },
  watch: {
    nav (val) {
      this.paraSucc = false
    }
  },
  methods: {
    navChange (nav) {
      this.nav = nav
    },
    handlePost (para) {
      if(para === false) {
        this.paraSucc = false
        return
      }
      para.method = this.nav
      axios.get('/api/para', {
        params: para
      }).then(res => {
        console.log('para res:',res)
        this.paraSucc = true
        this.newRes(res.data)
      }).catch(err => {
        console.log(err)
      })
    },
    ...mapMutations(['newRes'])
  }
}
</script>

<style lang="stylus" scoped>

</style>
