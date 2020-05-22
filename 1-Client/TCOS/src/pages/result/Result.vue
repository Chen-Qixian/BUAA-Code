<template>
  <div id="result">
    <common-header :cn="chinese"></common-header>
    <common-navbar @change="navChange" :itemList="itemList" :navId="nav" :cn="chinese">
      <li class="nav-item"></li>
    </common-navbar>
    <common-body :nav="nav">
      <result-preview v-if="nav === PREVIEW" @change="navChange"></result-preview>
      <result-file v-else-if="nav === FILE" @change="navChange"></result-file>
      <result-tree v-else-if="nav === TREE" @change="navChange"></result-tree>
      <result-heat v-else-if="nav === HEAT" @change="navChange"></result-heat>
      <result-task v-else-if="nav === TASK" @change="navChange"></result-task>
    </common-body>
    <common-footer></common-footer>
  </div>
</template>

<script>
import CommonHeader from '../common/Header'
import CommonNavbar from '../common/Navbar' 
import CommonBody from '../common/Body'
import CommonFooter from '../common/Footer'

import ResultPreview from './components/Preview'
import ResultFile from './components/FileDownload'
import ResultTree from './components/TreeData'
import ResultHeat from './components/HeatMap'
import ResultTask from './components/TaskMap'

import { mapState } from 'vuex'

export default {
  name: 'result',
  components: {
    CommonHeader,
    CommonNavbar,
    CommonBody,
    CommonFooter,
    ResultPreview,
    ResultFile,
    ResultTree,
    ResultHeat,
    ResultTask
  },
  data () {
    return {
      chinese: true,
      nav: 0,
      itemList: [
        {
          type: 'eye',
          msg:'结果预览'
        },
        {
          type: 'file-text',
          msg:'文件下载'
        },
        {
          type: 'apartment',
          msg:'节点树形图'
        },
        {
          type: 'dot-chart',
          msg:'通信热力图'
        },
        {
          type: 'swap',
          msg:'任务映射图'
        },
        {
          type: 'home',
          msg:'返回首页'
        }
      ],
      PREVIEW: 0,
      FILE: 1,
      TREE: 2,
      HEAT: 3,
      TASK: 4
    }
  },
  computed: {
    ...mapState({
      response: 'response'
    })
  },
  methods: {
    navChange (nav) {
      if(nav === 5) {
        this.$router.push('/')
      }
      this.nav = nav
    }
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
#result >>> .nav-item:last-child
  background $bgColor
  cursor auto
</style>
