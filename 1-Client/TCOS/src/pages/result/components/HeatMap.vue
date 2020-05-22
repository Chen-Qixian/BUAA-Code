<template>
  <div class="result-heat">
    <result-header>
      通信热力图
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
      <v-chart :options="heatMap" style="height: 500px;width: 75%;margin: 0 auto;"/>
    </result-body>
  </div>
</template>

<script>
import ResultHeader from './subcomponents/ResultHeader'
import ResultBody from './subcomponents/ResultBody'

import ECharts from 'vue-echarts'
import 'echarts/lib/chart/heatmap'
import 'echarts/lib/component/tooltip'
import 'echarts/lib/component/visualMap'

export default {
  name: 'ResultHeat',
  components: {
    ResultHeader,
    ResultBody,
    'v-chart': ECharts
  },
  data () {
    let [data,xData,yData] = this.$store.state.response.resultHeat

    return {
      heatMap: {
        tooltip: {},
        xAxis: {
          type: 'category',
          data: xData,
          axisLabel: {
            color: '#ccc',
            fontSize: 16
          }
        },
        yAxis: {
          type: 'category',
          data: yData,
          axisLabel: {
            color: '#ccc',
            fontSize: 16
          }
        },
        visualMap: {
          min: 0,
          max: 1,
          calculable: true,
          realtime: false,
          inRange: {
            color: ['#424242', '#4575b4', '#74add1', '#abd9e9', '#e0f3f8', '#ffffbf', '#fee090', '#fdae61', '#f46d43', '#d73027']
            // color: ['#424242','#4575b4', '#74add1', '#e0f3f8', '#ffffbf', '#fee090', '#fdae61', '#f46d43', '#d73027', '#a50026']
          }
        },
        series: [{
          name: '通信热力图',
          type: 'heatmap',
          data: data,
          emphasis: {
            itemStyle: {
              borderColor: '#333',
              borderWidth: 1,
              shadowBlur: {
                shadowColor: 'rgba(0, 0, 0, 0.5)',
                shadowBlur: 10
              }
            }
          },
          progressive: 200,
          progressiveThreshold: 1000,
          animation: true
        }]
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
.result-heat >>> .ant-btn
  border 3px solid
  margin-right 5px
  font-size 14px
  color $textColor
.btn:hover
  color $blue
</style>
