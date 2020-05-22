<template>
  <div class="result-task">
    <result-header>
      任务映射图
      <a-button 
        type="primary"
        icon="logout" 
        size="large"
        shape="round"
        ghost
        class="btn"
        @click="btnClick(0)"
      >
        返回结果预览
      </a-button>
    </result-header>
    <result-body>
      <v-chart theme="dark" :options="taskMap" style="height: 500px;width: 100%"/>
    </result-body>
  </div>
</template>

<script>
import ResultHeader from './subcomponents/ResultHeader'
import ResultBody from './subcomponents/ResultBody'

import ECharts from 'vue-echarts'
import 'echarts/lib/chart/graph'
import 'echarts/lib/component/title'
import 'echarts/lib/component/legend'
import 'echarts/lib/component/tooltip'

export default {
  name: 'ResultTask',
  components: {
    ResultHeader,
    ResultBody
  },
  data () {
    let graph = this.$store.state.response.resultTask
    let groupNum = graph.groupNum
    let categories = [];
    for (var i = 0; i < groupNum; i++) {
      categories[i] = {
        name: '进程组' + i
      };
    }
    
    return {
      taskMap: {
        title: {
          text: '进程-节点映射关系图',
          top: 'bottom',
          left: 'right',
          textStyle: {
            color: '#ccc'
          }
        },
        tooltip: {},
        legend: [{
          data: categories.map(function (a) {
            return a.name;
          })
        }],
        animation: false,
        series : [
          {
            name: '计算节点',
            type: 'graph',
            layout: 'force',
            data: graph.nodes,
            links: graph.links,
            categories: categories,
            roam: true,
            label: {
                position: 'right'
            },
            force: {
                initLayout: 'circular',
                repulsion: 50
            }
          }
        ]
      }
    }
  },
  methods: {
    newGraph (size) {
      let nodes = []
      let links = []
      for(let i = 0 ; i < size ; i ++) {
        let node = {}
        node.id = i
        node.name = `节点${i}`
        node.itemStyle = null
        node.symbolSize = 16
        node.value = Math.floor(Math.random() * 10) + 10
        node.category = i % 5 
        node.draggable = true
        nodes.push(node)
      }
      let cnt = 0
      for(let i = 0 ; i < size - 1; i ++) {
        for(let j = i + 1 ; j < size ; j ++) {
          if(i % 5 == j % 5 && i % 2 != 0 && j % 2 != 0) {
            let link = {}
            link.id = cnt ++
            link.source = i
            link.target = j
            link.name = null
            link.lineStyle = { normal: {} }
            links.push(link)
          }
          else if(i % 31 == j % 31) {
            let link = {}
            link.id = cnt ++
            link.source = i
            link.target = j
            link.name = null
            link.lineStyle = { normal: {} }
            links.push(link)
          }
        }
      }
      links = links.concat([{
        id: cnt ++,
        source: 10,
        target: 13,
        name: null,
        lineStyle: { normal: {} }
      },
      {
        id: cnt ++,
        source: 33,
        target: 36,
        name: null,
        lineStyle: { normal: {} }
      },
      {
        id: cnt ++,
        source: 14,
        target: 17,
        name: null,
        lineStyle: { normal: {} }
      },
      {
        id: cnt ++,
        source: 38,
        target: 8,
        name: null,
        lineStyle: { normal: {} }
      },
      {
        id: cnt ++,
        source: 18,
        target: 24,
        name: null,
        lineStyle: { normal: {} }
      },
      {
        id: cnt ++,
        source: 30,
        target: 24,
        name: null,
        lineStyle: { normal: {} }
      },
      {
        id: cnt ++,
        source: 28,
        target: 22,
        name: null,
        lineStyle: { normal: {} }
      },
      {
        id: cnt ++,
        source: 22,
        target: 40,
        name: null,
        lineStyle: { normal: {} }
      },
      {
        id: cnt ++,
        source: 32,
        target: 22,
        name: null,
        lineStyle: { normal: {} }
      }
      ])
      return {
        nodes,
        links
      }
    },
    btnClick (nav) {
      this.$emit('change', nav)
    }
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.result-task >>> .ant-btn
  border 3px solid
  margin-right 5px
  font-size 14px
  color $textColor
.btn:hover
  color $blue
</style>
