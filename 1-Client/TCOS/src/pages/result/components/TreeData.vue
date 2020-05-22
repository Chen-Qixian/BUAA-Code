<template>
  <div class="result-tree">
    <result-header>
      节点树形图
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
      <v-chart :options="treeChart" style="height: 500px;width: 100%"/>
    </result-body>
  </div>
</template>

<script>
import ResultHeader from './subcomponents/ResultHeader'
import ResultBody from './subcomponents/ResultBody'

import ECharts from 'vue-echarts'
import 'echarts/lib/chart/tree'
import 'echarts/lib/component/tooltip'

export default {
  name: 'ResultTree',
  components: {
    ResultHeader,
    ResultBody,
    'v-chart': ECharts
  },
  data () {
    let data = this.$store.state.response.resultTree
    
    data.children.forEach( function (datum, index) {
        index % 2 === 0 && (datum.collapsed = true);
    });
    return {
      treeChart: {
        tooltip: {
          trigger: 'item',
          triggerOn: 'mousemove'
        },
        series: [
          {
            type: 'tree',
            data: [data],
            left: '10%',
            // right: 0,
            top: 'center',
            // bottom: 0,
            width: '70%',
            height: '100%',
            symbol: 'emptyCircle',
            symbolSize: 16,
            label: {
              position: 'left',
              verticalAlign: 'middle',
              align: 'right',
              fontSize: 14,
              fontWeight: 'bold',
              color: '#f94',
              position:[0, 0]
            },
            lineStyle: {
              color: '#A69DFF',
              width: 2.5,
              curveness: 0.5
            },
            leaves: {
              label: {
                position: 'right',
                verticalAlign: 'middle',
                align: 'left'
              }
            },
            expandAndCollapse: true,
            animationDuration: 550,
            animationDurationUpdate: 750
          }
        ]
      }
    }
  },
  methods: {
    btnClick (nav) {
      this.$emit('change', nav)
    }
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.result-tree >>> .ant-btn
  border 3px solid
  margin-right 5px
  font-size 14px
  color $textColor
.btn:hover
  color $blue
</style>
